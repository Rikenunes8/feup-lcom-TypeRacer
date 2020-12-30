#include "../headers/sprite.h"
#include "../headers/graphic.h"
#include <stdarg.h>

static uint8_t BPP = 3;

Sprite * create_sprite(xpm_map_t xpm, int32_t x, int32_t y, int8_t x_speed, int8_t y_speed){
  Sprite* sprite = (Sprite*)malloc(sizeof(Sprite));
  xpm_image_t img;
  sprite->map = xpm_load(xpm, XPM_8_8_8, &img);
  sprite->width = img.width;
  sprite->height = img.height;
  sprite->x = x;
  sprite->y = y;
  sprite->xspeed = x_speed;
  sprite->yspeed = y_speed;
  return sprite;
}  

int animate_sprite(Sprite* sprite) 
{
  sprite->x += sprite->xspeed;
  sprite->y += sprite->yspeed;
  return 0;
}

int set_sprite(Sprite* sprite, uint16_t x, uint16_t y, int32_t x_speed, int32_t y_speed) {
  sprite->x = x;
  sprite->y = y;
  sprite->xspeed = x_speed;
  sprite->yspeed = y_speed;  
  return 0;
}

void destroy_sprite(Sprite *sprite) 
{
  if(sprite == NULL)
    return;
  if(sprite->map)
    free(sprite->map);
  free(sprite);
  sprite = NULL;
  return;
}


void draw_sprite(Sprite* sprite) {
  uint32_t color;
  for (int32_t i = 0; i < sprite->height; i++) {
    for (int32_t j = 0; j < sprite->width; j++) {
      // Set first byte of pixel's color
      color = sprite->map[(i*sprite->width + j)*BPP];
      // Set next bytes of pixel's color if it is more than 1 BPP
      for (uint32_t n = 1; n < BPP; n++) {
        color |= sprite->map[(i*sprite->width + j)*BPP + n]<<(8*n);
      }
      // If color is transparent don't draw it
      if (color != xpm_transparency_color(XPM_8_8_8))  
        graphic_pixel(sprite->x + j, sprite->y + i, color);
    }
  }
}

bool check_sp_collison(Sprite* sprite, int32_t x, int32_t y) {
  return  sprite->x < x && sprite->x+sprite->width > x &&
          sprite->y < y && sprite->y+sprite->height > y;
}





AnimSprite * create_asprite(int32_t x, int32_t y, int8_t x_speed, int8_t y_speed, uint8_t aspeed, uint8_t no_xpm, xpm_map_t xpm, ...) {
  AnimSprite *asp = malloc(sizeof(AnimSprite));
  asp->sp = create_sprite(xpm,x,y,x_speed,y_speed);
  asp->num_fig = no_xpm;
  asp->cur_fig = 0;
  asp->aspeed = aspeed;
  asp->map = malloc((no_xpm) * sizeof(uint8_t *));
  asp->map[0] = asp->sp->map;
  va_list ap;
  va_start(ap, xpm);
  for(uint8_t i = 1; i <no_xpm; i++ ) {
    xpm_map_t tmp = va_arg(ap, xpm_map_t);
    xpm_image_t img;
    asp->map[i] = xpm_load(tmp, XPM_8_8_8, &img);
  }
  va_end(ap);
  return asp;
}

void animate_asprite(AnimSprite *asp) {
  animate_sprite(asp->sp);

  asp->cur_aspeed = (asp->cur_aspeed+1)%asp->aspeed;

  if (asp->cur_aspeed == 0)
    asp->cur_fig = (asp->cur_fig+1)%asp->num_fig;
}

void set_asprite(AnimSprite* asp, uint8_t aspeed, uint8_t cur_aspeed, uint8_t num_fig) {
  asp->aspeed = aspeed;
  asp->cur_aspeed = cur_aspeed;
  asp->num_fig = num_fig;
}

void set_asprite_sprite(AnimSprite* asp, uint16_t x, uint16_t y, int32_t x_speed, int32_t y_speed) {
  set_sprite(asp->sp, x, y, x_speed, y_speed);
}

void destroy_asprite(AnimSprite *asp) {
  free(asp->map);
  destroy_sprite(asp->sp);
  free(asp);
}

void draw_asprite(AnimSprite* asp) {
  uint32_t color;
  for (int32_t i = 0; i < asp->sp->height; i++) {
    for (int32_t j = 0; j < asp->sp->width; j++) {
      // Set first byte of pixel's color
      color = asp->map[asp->cur_fig][(i*asp->sp->width + j)*BPP];
      // Set next bytes of pixel's color if it is more than 1 BPP
      for (uint32_t n = 1; n < BPP; n++) {
        color |= asp->map[asp->cur_fig][(i*asp->sp->width + j)*BPP + n]<<(8*n);
      }
      // If color is transparent don't draw it
      if (color != xpm_transparency_color(XPM_8_8_8))  
        graphic_pixel(asp->sp->x + j, asp->sp->y + i, color);
    }
  }
}

bool check_asp_collison(AnimSprite* asp, int32_t x, int32_t y) {
  return check_sp_collison(asp->sp, x, y);
}
