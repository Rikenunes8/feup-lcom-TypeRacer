#include <graphic.h>

static void *video_mem;
static uint32_t h_res;
static uint32_t v_res;
static uint32_t bits_per_pixel;

int graphic_init(uint16_t mode, vbe_mode_info_t *info) {
    // Define characteristics of the mode static.
    h_res = info->XResolution; 
    v_res = info->YResolution;
    bits_per_pixel = info->BitsPerPixel;

    struct minix_mem_range mr;
    mr.mr_base = info->PhysBasePtr; // Start of virtual memory
    unsigned vram_size = h_res * v_res * (bits_per_pixel+7)/8;
    mr.mr_limit = mr.mr_base + vram_size; // End of virtual memory

    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != OK) 
        printf("Error sys_privctl\n");
    video_mem = vm_map_phys(SELF, (void*)mr.mr_base, vram_size);
    
    reg86_t r;
    memset(&r, 0, sizeof(r)); //clears r
    r.ax = VBE_FUNCTION | SET_VBE_MODE; // VBE call, function 02 -- set VBE mode
    r.bx = BIT(14) | mode; // set bit 14: linear framebuffer
    r.intno = 0x10;
    if(sys_int86(&r) != OK) 
    {
        printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }
    return 0;
}

int graphic_pixel(uint32_t x, uint32_t y, uint32_t color) {
    return 0;
}
