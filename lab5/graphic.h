#ifndef _LCOM_GRAPHIC_H
#define _LCOM_GRAPHIC_H

#include <lcom/lcf.h>
#include <machine/int86.h>
#include <vbe.h>

int graphic_init(uint16_t mode, vbe_mode_info_t *info, uint8_t vbe_function);

int graphic_pixel(uint32_t x, uint32_t y, uint32_t color);


#endif /* _LCOM_VG_H */

