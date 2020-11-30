#include <graphic.h>

static char *video_mem;  /* Address to which VRAM is mapped */
static uint32_t h_res;  /* Frame horizontal (x) resolution */
static uint32_t v_res;  /* Frame vertical (y) resolution */
static uint32_t bits_per_pixel;
static uint8_t red_mask_size;
static uint8_t green_mask_size;
static uint8_t blue_mask_size;
static uint8_t red_position;
static uint8_t green_position;
static uint8_t blue_position;

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
    red_mask_size = info->RedMaskSize;
    green_mask_size = info->GreenMaskSize;
    blue_mask_size = info->BlueMaskSize;
    red_position = info->RedFieldPosition;
    green_position = info->GreenFieldPosition;
    blue_position = info->BlueFieldPosition;

    printf("red_size %d\n", red_mask_size);
    printf("green_size %d\n", green_mask_size);
    printf("blue_size %d\n", blue_mask_size);

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

int graphic_xpm(xpm_map_t xpm, uint16_t x, uint16_t y, bool trans) {
  if (!trans)
    printf("graphic\n");
  xpm_image_t img;
  uint8_t *map;
  map = xpm_load(xpm, XPM_INDEXED, &img);

  for (uint16_t i = 0; i < img.height; i++) {
    for (uint16_t j = 0; j < img.width; j++) {
      if (!trans)  
        graphic_pixel(x + j, y + i, map[i*img.width + j]);
      else
        graphic_pixel(x + j, y + i, xpm_transparency_color(XPM_INDEXED));
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
    r.ax = 0x4F00;
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


int vg_draw_pattern(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step)
{
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t width = h_res/no_rectangles;
    uint16_t height = v_res/no_rectangles;
    uint32_t color = 0;
    uint8_t red_part= first>>red_position;
    uint8_t green_part= first>>green_position;
    uint8_t blue_part= first>>blue_position;
    uint8_t red, green, blue;

    if(mode == 0x105)
        for(int stripe_y = 0; stripe_y < no_rectangles; stripe_y++)
        {
            for(int stripe_x = 0; stripe_x < no_rectangles; stripe_x++)
            {
                color = (first + (stripe_y * no_rectangles + stripe_x) * step) % (1 << bits_per_pixel);
                vg_draw_rectangle(x, y, width, height, color);
                x = x + width;
            }
            y = y + height;
        }
    else //direct-color models
    {
        for(int stripe_y = 0; stripe_y < no_rectangles; stripe_y++)
        {
            for(int stripe_x = 0; stripe_x < no_rectangles; stripe_x++)
            {
                red = (red_part + stripe_x * step) % (1 << red_mask_size);
                green = (green_part + stripe_y * step) % (1 << green_mask_size);
                blue = (blue_part + (stripe_x + stripe_y) * step) % (1 << blue_mask_size);
                color = red<<red_position | green<<green_position | blue<<blue_position;
                vg_draw_rectangle(x, y, width, height, color);
                x = x + width;
            }
            y = y + height;
        }
        
    }

        return 0;
}
