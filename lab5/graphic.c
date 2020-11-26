#include <graphic.h>

static char *video_mem;  /* Address to which VRAM is mapped */
static uint32_t h_res;  /* Frame horizontal (x) resolution */
static uint32_t v_res;  /* Frame vertical (y) resolution */
static uint32_t bits_per_pixel;

int graphic_init(uint16_t mode, vbe_mode_info_t *info, uint8_t vbe_function) 
{
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
    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != OK) 
        printf("Error sys_privctl\n");

    /* Map memory */
    video_mem = vm_map_phys(SELF, (void*)mr.mr_base, vram_size); //endereço base 

    if(video_mem == MAP_FAILED)
        panic("couldn't map video memory");
    
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
