#ifndef _LCOM_GRAPHIC_H
#define _LCOM_GRAPHIC_H

#include <lcom/lcf.h>
#include <machine/int86.h>
#include <../headers/vbe.h>

int graphic_get_mode_info(uint16_t mode, vbe_mode_info_t *info);

int graphic_def(vbe_mode_info_t *info);

int graphic_init(uint16_t mode, uint8_t vbe_function);

int graphic_pixel(uint32_t x, uint32_t y, uint32_t color);

int graphic_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int graphic_xpm(char** xpm, uint16_t x, uint16_t y);

int graphic_cntrl_info(vg_vbe_contr_info_t *info);

void fr_buffer_to_video_mem();

void graphic_set_background(uint32_t color);

uint8_t get_char_xpm(char c);

uint32_t get_h_res();

uint32_t get_v_res();

void destroy_fr_bufffer();

#endif /* _LCOM_VG_H */

