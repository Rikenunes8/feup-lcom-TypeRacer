#include <graphic.h>

static char *video_mem;  /* Address to which VRAM is mapped */
static uint32_t h_res;  /* Frame horizontal (x) resolution */
static uint32_t v_res;  /* Frame vertical (y) resolution */
static uint32_t bits_per_pixel;

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

    struct minix_mem_range mr;
    unsigned int vram_size;  // VRAM's size, but you can use the frame-buffer size, instead 		    

    mr.mr_base = info->PhysBasePtr; // Start of virtual memory
    vram_size = h_res * v_res * (bits_per_pixel+7)/8;
    mr.mr_limit = mr.mr_base + vram_size; // End of virtual memory

    //get permissions
    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != OK) {
        printf("Error sys_privctl\n");
        return 1;
    }

    /* Map memory */
    video_mem = vm_map_phys(SELF, (void*)mr.mr_base, vram_size); //endereço base 

    if(video_mem == MAP_FAILED)
        panic("couldn't map video memory");
    return 0;
}

int graphic_init(uint16_t mode, uint8_t vbe_function) 
{
    reg86_t r;
    memset(&r, 0, sizeof(r)); //clears r
    r.ax = VBE_FUNCTION | vbe_function; // VBE call, function 02 -- set VBE mode
    r.bx = BIT(14) | mode; // set bit 14: linear framebuffer
    r.intno = 0x10;
    if(sys_int86(&r) != OK) 
    {
        printf("sys_int86() failed \n");
        return 1;
    }
    return 0;
}

int graphic_pixel(uint32_t x, uint32_t y, uint32_t color) {
    uint8_t BPP = (bits_per_pixel+7)/8;
    char* dest = video_mem;
    dest += (y*h_res + x)*BPP;
    for (uint8_t i = 0; i < BPP; i++) {
        *(dest+i) = (uint8_t)color;
        color = color >> 8;
    }
    //memcpy(&video_mem[((y*h_res)+x)*BPP], &color, BPP); 

    return 0;
}

int graphic_xpm(xpm_map_t xpm, uint16_t x, uint16_t y, bool trans, uint32_t color) {
  xpm_image_t img;
  uint8_t *map;
  map = xpm_load(xpm, XPM_INDEXED, &img);

  for (uint16_t i = 0; i < img.height; i++) {
    for (uint16_t j = 0; j < img.width; j++) {
      if (!trans)  
        graphic_pixel(x + j, y + i, map[i*img.width + j]);
      else
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
    //r.ax = VBE_FUNCTION | RET_VBE_CONTROLLER; // VBE call, function 00h -- ret VBE controller information
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

uint8_t get_char_xpm(char c) {
    switch (c) {
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        case 'A': return 10;
        case 'B': return 11;
        case 'C': return 12;
        case 'D': return 13;
        case 'E': return 14;
        case 'F': return 15;
        case 'G': return 16;
        case 'H': return 17;
        case 'I': return 18;
        case 'J': return 19;
        case 'K': return 20;
        case 'L': return 21;
        case 'M': return 22;
        case 'N': return 23;
        case 'O': return 24;
        case 'P': return 25;
        case 'Q': return 26;
        case 'R': return 27;
        case 'S': return 28;
        case 'T': return 29;
        case 'U': return 30;
        case 'V': return 31;
        case 'W': return 32;
        case 'X': return 33;
        case 'Y': return 34;
        case 'Z': return 35;
        case 'a': return 36;
        case 'b': return 37;
        case 'c': return 38;
        case 'd': return 39;
        case 'e': return 40;
        case 'f': return 41;
        case 'g': return 42;
        case 'h': return 43;
        case 'i': return 44;
        case 'j': return 45;
        case 'k': return 46;
        case 'l': return 47;
        case 'm': return 48;
        case 'n': return 49;
        case 'o': return 50;
        case 'p': return 51;
        case 'q': return 52;
        case 'r': return 53;
        case 's': return 54;
        case 't': return 55;
        case 'u': return 56;
        case 'v': return 57;
        case 'w': return 58;
        case 'x': return 59;
        case 'y': return 60;
        case 'z': return 61;
        case ' ': return 62;
        case ':': return 63;
        case ';': return 64;
        case ',': return 65;
        case '.': return 66;
        case '!': return 67;
        case '?': return 68;
        case '\'': return 69;
        case '\"': return 70;
        default: return 0;
    }
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
    for(int j=0; j < len; j++)
    {
        graphic_pixel(x+j, y, color);
    }
    return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) 
{
    //printf("BPP: %d\n", (bits_per_pixel+7)/8);
    //printf("video_mem: %x\n", video_mem);
    for(int i=0; i < height; i++)
    {
        vg_draw_hline(x, y+i, width, color);
    }
    return 0;
}	

