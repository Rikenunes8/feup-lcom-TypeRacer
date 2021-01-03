#ifndef _LCOM_GRAPHIC_H
#define _LCOM_GRAPHIC_H

#include <lcom/lcf.h>
#include <machine/int86.h>
#include <../headers/vbe.h>

/** @defgroup graphic graphic
 * @{
 *
 * Graphic related functions, variables and data structures
 */

#define WHITE   0xFFFFFF  //!< 24 bits color white 
#define BLACK   0x000000  //!< 24 bits color black 

/**
 * @brief Gets VBE mode info
 * 
 * @param mode 16-bit VBE mode
 * @param info Structure to be set with mode info
 * @return int 0 if success, 1 otherwise
 */
int graphic_get_mode_info(uint16_t mode, vbe_mode_info_t *info);
/**
 * @brief Configures mode settings
 * 
 * @param info Structure with mode info
 * @return int 0 if success, 1 otherwise
 */
int graphic_def(vbe_mode_info_t *info);
/**
 * @brief Initializes VBE mode
 * 
 * @param mode 16-bit VBE mode to be set
 * @return int 0 if success, 1 otherwise
 */
int graphic_init(uint16_t mode);
/**
 * @brief Draws a pixel in frame buffer
 * 
 * @param x X position of the pixel to be drawn
 * @param y Y position of the pixel to be drawn
 * @param color Color of the pixel to be drawn
 * @return int 0 if success, 1 if out of mode bounderies
 */
int graphic_pixel(uint32_t x, uint32_t y, uint32_t color);
/**
 * @brief Draws a rectangle in frame buffer
 * 
 * @param x X position of the top-left corner
 * @param y Y position of the top-left corner
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 * @param color Color of the rectangle
 * @return int 0 if success
 */
int graphic_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
/**
 * @brief Loads a xpm
 * 
 * @param map Map of the xpm to be set
 * @param img Xpm info to be set
 * @param type Color mode of xpm
 * @param xpm XPM to be load
 * @return int 0 if success
 */
int graphic_xpm_load(uint8_t ** map, xpm_image_t *img, enum xpm_image_type type, xpm_map_t xpm);
/**
 * @brief Draws a xpm in frame buffer
 * 
 * @param map Map of the xpm to be draw
 * @param img Xpm info
 * @param x X position of the top-left corner
 * @param y Y position of the top-left corner
 * @return int 0 if success
 */
int graphic_xpm(uint8_t *map, xpm_image_t *img, uint16_t x, uint16_t y);

int graphic_cntrl_info(vg_vbe_contr_info_t *info);
/**
 * @brief Copies frame buffer to virtual memory. Draws frame buffer in the screen
 * 
 */
void fr_buffer_to_video_mem();
/**
 * @brief Copies auxiliary buffer to frame buffer
 * 
 * @param aux Buffer to be copied
 */
void aux_to_fr_buffer(char* aux);
/**
 * @brief Copies frame buffer to auxiliary buffer
 * 
 * @param aux Buffer to be copied from frame buffer
 */
void fr_buffer_to_aux(char* aux);
/**
 * @brief Sets all frame buffer with one color
 * 
 * @param color Color
 */
void graphic_set_background(uint32_t color);

/**
 * @brief Gets the height of VBE mode
 * 
 * @return uint32_t Height
 */
uint32_t get_h_res();
/**
 * @brief Gets the width of VBE mode
 * 
 * @return uint32_t Width
 */
uint32_t get_v_res();
/**
 * @brief Gets number of bytes per pixel in VBE mode set
 * 
 * @return uint32_t Bytes per pixel
 */
uint32_t get_BPP();
/**
 * @brief Frees frame buffer allocated in graphic_def() function
 * 
 */
void destroy_fr_bufffer();

/** @} end of graphic */

#endif /* _LCOM_VG_H */

