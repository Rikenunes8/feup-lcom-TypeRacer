#ifndef _LCOM_GRAPHIC_H
#define _LCOM_GRAPHIC_H

#include <lcom/lcf.h>
#include <machine/int86.h>
#include <vbe.h>

int graphic_def(vbe_mode_info_t *info);

int graphic_init(uint16_t mode, uint8_t vbe_function);

int graphic_pixel(uint32_t x, uint32_t y, uint32_t color);

int graphic_xpm(xpm_map_t xpm, uint16_t x, uint16_t y);

#endif /* _LCOM_VG_H */

