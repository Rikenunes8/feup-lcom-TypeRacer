#include <lcom/lcf.h>

#include "../headers/Chars.h"

#include "../headers/keyboard.h"
#include "../headers/graphic.h"
#include "../xpm/letters.h"


static size_t no_chars = 75;
static char list_chars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
                            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                            ' ', ':', ';', ',', '.', '!', '?', '\'', '\"', '%', '/', '-', '_'};

static uint8_t **letters_maps;
static uint32_t BPP = 3;

static bool shift_right = false;
static bool shift_left = false;



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

int convert_text_Char_to_text(char* text, Char* text_Char, size_t len) {
  for (size_t i = 0; i < len; i++) {
    text[i] = list_chars[text_Char[i].index];
  }
  text[len] = '\0';
  return 0;
}

uint16_t convert_text_to_text_char(const char* text, Char* text_Char, size_t len, uint16_t x_position, uint16_t y_position) {
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
  display_text_Char(text_Char, len);
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
  uint32_t color;
  for (uint16_t i = 0; i < CHAR_H; i++) {
    for (uint16_t j = 0; j < CHAR_W; j++) {
      // Set first byte of pixel's color
      color = letters_maps[c->index][(i*CHAR_W + j)*BPP];
      // Set next bytes of pixel's color if it is more than 1 BPP
      for (uint32_t n = 1; n < BPP; n++) {
        color |= letters_maps[c->index][(i*CHAR_W + j)*BPP + n]<<(8*n);
      }
      if (c->state == WRONG && color == 0x000000)
        color = 0xFF0000;
      else if (c->state == RIGHT && color == 0x000000)
        color = 0x00FF00;
      // If color is transparent don't draw it
      if (color != xpm_transparency_color(XPM_8_8_8))  
        graphic_pixel(c->posx + j, c->posy + i, color);
    }
  }     
}

uint8_t get_scancode_char(uint8_t *bytes) {
  //indicates that is a two_byte scancode
  if (bytes[0] == 0xE0) {
    switch (bytes[1]) {
      case 0x4b: return L_ARROW; //left arrow
      case 0x4d: return R_ARROW; //right arrow
      case 0x48: return T_ARROW; //top arrow
      case 0x50: return D_ARROW; //down arrow
    }
  }
  else 
  {
    //scancodes when shift key is not pressed
    if (!shift_right && !shift_left) 
    {
      switch (bytes[0]) {
        case 0x0b: return 0; // '0'
        case 0x02: return 1; // '1'
        case 0x03: return 2;
        case 0x04: return 3;
        case 0x05: return 4;
        case 0x06: return 5;
        case 0x07: return 6;
        case 0x08: return 7;
        case 0x09: return 8;
        case 0x0a: return 9;
        case 0x1e: return 36; // 'a'
        case 0x30: return 37;
        case 0x2e: return 38;
        case 0x20: return 39;
        case 0x12: return 40;
        case 0x21: return 41;
        case 0x22: return 42;
        case 0x23: return 43;
        case 0x17: return 44;
        case 0x24: return 45;
        case 0x25: return 46;
        case 0x26: return 47;
        case 0x32: return 48;
        case 0x31: return 49;
        case 0x18: return 50;
        case 0x19: return 51;
        case 0x10: return 52;
        case 0x13: return 53;
        case 0x1f: return 54;
        case 0x14: return 55;
        case 0x16: return 56;
        case 0x2f: return 57;
        case 0x11: return 58;
        case 0x2d: return 59;
        case 0x15: return 60;
        case 0x2c: return 61;
        case 0x33: return 65; // ','
        case 0x34: return 66; // '.'
        case 0x0c: return 69; // '''
        case 0x35: return 73; // '-'
      }
    }
    else 
    {
      //scancodes when shift key is pressed
      switch (bytes[0]) {
        case 0x1e: return 10; // 'A'
        case 0x30: return 11;
        case 0x2e: return 12;
        case 0x20: return 13;
        case 0x12: return 14;
        case 0x21: return 15;
        case 0x22: return 16;
        case 0x23: return 17;
        case 0x17: return 18;
        case 0x24: return 19;
        case 0x25: return 20;
        case 0x26: return 21;
        case 0x32: return 22;
        case 0x31: return 23;
        case 0x18: return 24;
        case 0x19: return 25;
        case 0x10: return 26;
        case 0x13: return 27;
        case 0x1f: return 28;
        case 0x14: return 29;
        case 0x16: return 30;
        case 0x2f: return 31;
        case 0x11: return 32;
        case 0x2d: return 33;
        case 0x15: return 34;
        case 0x2c: return 35; // 'Z'
        case 0x34: return 63; // ':'
        case 0x33: return 64; // ';'
        case 0x02: return 67; // '!'
        case 0x0c: return 68; // '?'
        case 0x03: return 70; // '"'
        case 0x06: return 71; // '%'
        case 0x08: return 72; // '/'
        case 0x35: return 74; // '_'
      }
    }

    //other scancodes
    switch (bytes[0]) {
      case 0x39: return SPACE; // ' '
      case 0x36: shift_left = true; return NOTHING;
      case 0x2a: shift_right = true; return NOTHING;
      case 0xb6: shift_left = false; return NOTHING;
      case 0xaa: shift_right = false; return NOTHING;
      case 0x0e: return BACKSPACE; // backspace
      case 0x81: return ESC;
      default: return NOTHING;
    }
  }
  return NOTHING;
}


