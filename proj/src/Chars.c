#include "../headers/Chars.h"
#include "../headers/keyboard.h"


static size_t no_chars = 75;
static char list_chars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
                            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                            ' ', ':', ';', ',', '.', '!', '?', '\'', '\"', '%', '/', '-', '_'};

static uint8_t **letters_maps;
static uint32_t BPP = 3;



int Chars_init() {
  // Load letters xpms
  extern xpm_map_t letters[];
  letters_maps =(uint8_t**)malloc(no_chars*sizeof(uint8_t*));
  xpm_image_t img;
  for (size_t i = 0; i < no_chars;i++) {
    letters_maps[i] = (uint8_t*)malloc(CHAR_W*CHAR_H*3);
    graphic_xpm_load(&letters_maps[i], &img, XPM_8_8_8, letters[i]);
  }
  return 0;
}

int Chars_end() {
  for (size_t i = 0; i < no_chars; i++)
    free(letters_maps[i]);
  free(letters_maps);
  return 0;
}

int graphic_Char_xpm(uint8_t *map, uint16_t x, uint16_t y, Char_state state) {
  uint32_t color;
  for (uint16_t i = 0; i < CHAR_H; i++) {
    for (uint16_t j = 0; j < CHAR_W; j++) {
      // Set first byte of pixel's color
      color = map[(i*CHAR_W + j)*BPP];
      // Set next bytes of pixel's color if it is more than 1 BPP
      for (uint32_t n = 1; n < BPP; n++) {
        color |= map[(i*CHAR_W + j)*BPP + n]<<(8*n);
      }
      if (state == WRONG && color == 0x000000)
        color = 0xFF0000;
      else if (state == RIGHT && color == 0x000000)
        color = 0x00FF00;
      // If color is transparent don't draw it
      if (color != xpm_transparency_color(XPM_8_8_8))  
        graphic_pixel(x + j, y + i, color);
    }
  }     
  return 0;

}

int convert_text_Char_to_text(char* text, Char* text_Char, size_t len) {
  for (size_t i = 0; i < len; i++) {
    text[i] = list_chars[text_Char[i].index];
  }
  text[len] = '\0';
  return 0;
}

int convert_text_to_text_char(const char* text, Char* text_Char, size_t len, uint16_t x_position, uint16_t y_position) {
  // Set chars to be drawn
  uint16_t x = 0;
  uint16_t y = 0;  
  for (size_t i = 0; i < len; i++) {
    // Set index of char draw in letters
    text_Char[i].index = get_char_xpm(text[i]);
    text_Char[i].state = NORMAL;
    
    // Set position where to be drawn
    text_Char[i].posx = x_position + x*(CHAR_W+2);
    text_Char[i].posy = y_position + y*(CHAR_H+3);
    x++; // Next horizontal position
    // If the char is ' ' and x passed the limit set next vertical position
    if (x_position+x*(CHAR_W+2)>get_h_res()-100 && text_Char[i].index == SPACE) {
        y++; 
        x = 0;
    }
  }
  return y+1;
}

int display_text(const char* text, Char* text_Char, size_t len, uint16_t x_position, uint16_t y_position) 
{
  uint16_t y = convert_text_to_text_char(text, text_Char, len, x_position, y_position);
  // Draw text
  for (size_t n = 0; n < len; n++) {
    display_Char(&text_Char[n]);
  }
  return y;
}

int display_text_Char(Char* text_Char, size_t len) {
  for (size_t i = 0; i < len; i++) {
    display_Char(&text_Char[i]);
  }
  return 0;
}

void display_integer(int integer, uint16_t x, uint16_t y) {
  char str[20];    //empty string to store no_seconds

  sprintf(str, "%d", integer); //casts the number no_seconds to the string str
  
  Char* str_Char = malloc(strlen(str)*sizeof(Char)); // Convert string of chars to string of Chars

  display_text(str, str_Char, strlen(str), x, y);

  free(str_Char);
}

void display_float(float decimal, uint16_t x, uint16_t y) 
{
  char str[20];    //empty string to store no_seconds

  sprintf(str, "%.2f", decimal); //casts the number no_seconds to the string str
  
  Char* str_Char = malloc(strlen(str)*sizeof(Char)); // Convert string of chars to string of Chars

  display_text(str, str_Char, strlen(str), x, y);

  free(str_Char);
}

void display_time(uint16_t seconds, uint16_t x, uint16_t y) {
  char time[20]; 
  sprintf(time, "%d : %d ", seconds/60, seconds%60); 
  
  Char* time_Char = malloc(strlen(time)*sizeof(Char)); // Convert string of chars to string of Chars

  display_text(time, time_Char, strlen(time), x, y);

  free(time_Char);
}

void display_Char(Char *c) {
  graphic_Char_xpm(letters_maps[c->index], c->posx, c->posy, c->state);
}
