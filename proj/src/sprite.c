#include "../headers/sprite.h"
#include "../headers/graphic.h"

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


void draw_sprite(Sprite* sprite, int32_t x, int32_t y) {
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
        graphic_pixel(x + j, y + i, color);
    }
  }
}

bool check_collison(Sprite* sprite, int32_t x, int32_t y) {
  return  sprite->x < x && sprite->x+sprite->width > x &&
          sprite->y < y && sprite->y+sprite->height > y;
}





AnimSprite * create_asprite(char *base, char *pic1[], ...);

int animate_asprite(AnimSprite *fig, char *base);

void destroy_asprite(AnimSprite *fig, char *base);

