#ifndef _LCOM_GRAPHIC_H
#define _LCOM_GRAPHIC_H

#include <lcom/lcf.h>
#include <machine/int86.h>
#include <../headers/vbe.h>

#define CHAR_W  10
#define CHAR_H  14
#define WHITE   0xFFFFFF
#define BLACK   0x000000
#define KEY_BAR 72

typedef enum {
  NORMAL,
  WRONG,
  RIGHT
} Char_state;

/* Represents a drawn char
 * index: number corresponding to a char
 * posx: x position where the char is drawn
 * posy: y position where the char is drawn 
 */
typedef struct {
    uint8_t index;
    uint16_t posx;
    uint16_t posy;
    Char_state state;
} Char;

int graphic_get_mode_info(uint16_t mode, vbe_mode_info_t *info);

int graphic_def(vbe_mode_info_t *info);

int graphic_init(uint16_t mode);

int graphic_pixel(uint32_t x, uint32_t y, uint32_t color);

int graphic_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int graphic_xpm_load(uint8_t ** map, xpm_image_t *img, enum xpm_image_type type, xpm_map_t xpm);

int graphic_xpm(uint8_t *map, xpm_image_t *img, uint16_t x, uint16_t y);

int graphic_Char_xpm(uint8_t *map, uint16_t x, uint16_t y, Char_state state);

int graphic_cntrl_info(vg_vbe_contr_info_t *info);

void fr_buffer_to_video_mem();

void graphic_set_background(uint32_t color);

uint8_t get_char_xpm(char c);

uint32_t get_h_res();

uint32_t get_v_res();

void destroy_fr_bufffer();

#endif /* _LCOM_VG_H */

