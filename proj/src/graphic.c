#include <../headers/graphic.h>

static char *video_mem;  /* Address to which VRAM is mapped */
static char *fr_buffer;  /* Address to auxiliar buffer */
static uint8_t current_page; /* 0 -> front page; 1 -> back page */

static uint32_t h_res;  /* Frame horizontal (x) resolution */
static uint32_t v_res;  /* Frame vertical (y) resolution */
static uint32_t bits_per_pixel; /* Bits per pixel */
static uint32_t BPP;    /* Bytes per pixel */

int graphic_get_mode_info(uint16_t mode, vbe_mode_info_t *info) {
    mmap_t map;
    void* ptr = lm_alloc(sizeof(info), &map);

    reg86_t r;
    memset(&r, 0, sizeof(r)); //clears r
    //r.ax = VBE_FUNCTION | RET_VBE_CONTROLLER; // VBE call, function 00h -- ret VBE controller information
    r.ax = VBE_FUNCTION | RET_VBE_MODE;
    r.es = PB2BASE(map.phys);
    r.di = PB2OFF(map.phys);
    r.cx = mode;
    r.intno = 0x10;

    if(sys_int86(&r) != OK) {
        printf("sys_int86() failed \n");
        return 1;
    }

    memcpy(info, ptr, sizeof(*info));
    lm_free(&map);

    return 0;
}

int graphic_def(vbe_mode_info_t *info) {
    // Define characteristics of the mode static.
    h_res = info->XResolution; 
    v_res = info->YResolution;
    bits_per_pixel = info->BitsPerPixel;
    BPP = (bits_per_pixel+7)/8;

    struct minix_mem_range mr;
    unsigned int vram_size;  // VRAM's size, but you can use the frame-buffer size, instead 		    

    mr.mr_base = info->PhysBasePtr; // Start of virtual memory
    vram_size = h_res * v_res * BPP;
    mr.mr_limit = mr.mr_base + vram_size*2; // End of virtual memory

    //get permissions
    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != OK) {
        printf("Error sys_privctl\n");
        return 1;
    }

    /* Map memory */
    video_mem = vm_map_phys(SELF, (void*)mr.mr_base, vram_size*2); //endereço base 
    fr_buffer = video_mem + vram_size;
    current_page = 0;

    if(video_mem == MAP_FAILED)
        panic("couldn't map video memory");
    return 0;
}

int graphic_init(uint16_t mode) 
{
    reg86_t r;
    memset(&r, 0, sizeof(r)); //clears r
    r.ax = VBE_FUNCTION | SET_VBE_MODE; // VBE call, function 02 -- set VBE mode
    r.bx = BIT(14) | mode; // set bit 14: linear framebuffer
    r.intno = 0x10;
    if(sys_int86(&r) != OK) 
    {
        printf("sys_int86() failed \n");
        return 1;
    }
    return 0;
}

int graphic_flip_page() {
  reg86_t r;
  memset(&r, 0, sizeof(r));
  r.ax = VBE_FUNCTION | SET_DIS_START;
  r.bl = SET_DS;
  r.dx = current_page * v_res;
  r.intno = 0x10;

  if(sys_int86(&r) != OK) 
  {
    printf("sys_int86() failed \n");
    return 1;
  }

  current_page = (current_page+1)%2;
  fr_buffer = video_mem + h_res * v_res * BPP * current_page;

  return 0;
}

int graphic_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x>h_res || y>v_res) return 1;

    /* Outro método
    char* dest = fr_buffer;
    dest += (y*h_res + x)*BPP;
    for (uint8_t i = 0; i < BPP; i++) {
        *(dest+i) = (uint8_t)color;
        color = color >> 8;
    }*/

    //memcpy(&video_mem[((y*h_res)+x)*BPP], &color, BPP); 
    memcpy(&fr_buffer[((y*h_res)+x)*BPP], &color, BPP); 

    return 0;
}

int graphic_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) 
{
  for(int i=0; i < height; i++){
    for (int j=0; j < width; j++)
      graphic_pixel(x+j, y+i, color);
  }
  return 0;
}	
	

int graphic_xpm_load(uint8_t ** map, xpm_image_t *img, enum xpm_image_type type, xpm_map_t xpm) {
  *map = xpm_load(xpm, type, img);
  return 0;
}

int graphic_xpm(uint8_t *map, xpm_image_t *img, uint16_t x, uint16_t y) {
  uint32_t color;
  for (uint16_t i = 0; i < img->height; i++) {
    for (uint16_t j = 0; j < img->width; j++) {
      // Set first byte of pixel's color
      color = map[(i*img->width + j)*BPP];
      // Set next bytes of pixel's color if it is more than 1 BPP
      for (uint32_t n = 1; n < BPP; n++) {
        color |= map[(i*img->width + j)*BPP + n]<<(8*n);
      }
      // If color is transparent don't draw it
      if (color != xpm_transparency_color(img->type))  
        graphic_pixel(x + j, y + i, color);
    }
  }     
  return 0;
}

int graphic_cntrl_info(vg_vbe_contr_info_t *info) {
    
    mmap_t map;
    void* ptr = lm_alloc(sizeof(info), &map);
    ((char*)ptr)[0] = 'V';
    ((char*)ptr)[1] = 'B';
    ((char*)ptr)[2] = 'E';
    ((char*)ptr)[3] = '2';

    reg86_t r;
    memset(&r, 0, sizeof(r)); //clears r
    r.ax = VBE_FUNCTION|RET_VBE_CONTROLLER;
    r.es = PB2BASE(map.phys);
    r.di = PB2OFF(map.phys);
    r.intno = 0x10;

    if(sys_int86(&r) != OK) {
        printf("sys_int86() failed \n");
        return 1;
    }

    memcpy(info, ptr, sizeof(*info));
    lm_free(&map);

    return 0;

}

void fr_buffer_to_video_mem() {
  memcpy(video_mem, fr_buffer, h_res*v_res*BPP);
}

void aux_to_fr_buffer(char* aux) {
  memcpy(fr_buffer, aux, h_res*v_res*BPP);
}

void fr_buffer_to_aux(char* aux) {
  memcpy(aux, fr_buffer, h_res*v_res*BPP);
}

void graphic_set_background(uint32_t color) {
  memset(fr_buffer, color, h_res*v_res*BPP);
}

uint32_t get_h_res() {return h_res;}

uint32_t get_v_res() {return v_res;}

uint32_t get_BPP() {return BPP;}

