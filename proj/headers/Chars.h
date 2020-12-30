#ifndef _LCOM_CHARS_H
#define _LCOM_CHARS_H

#include <lcom/lcf.h>
#include "../headers/graphic.h"
#include "../xpm/letters.h"



#define CHAR_W  10
#define CHAR_H  14


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

int Chars_init();

int Chars_end();


int graphic_Char_xpm(uint8_t *map, uint16_t x, uint16_t y, Char_state state);

int convert_text_Char_to_text(char* text, Char* text_Char, size_t len);

/**
 * Convert array of chars to array of Chars
 */ 
int convert_text_to_text_char(const char* text, Char* text_Char, size_t len, uint16_t x_position, uint16_t y_position);

/**
 * Displays text defined
 * @param text: text to display on the screen
 * @param text_Char: place to where convert text in order to be drawable 
 * @param len: lenght of text
 * @param x_position: x position where start writing the text
 * @param y_position: y position where start writing the text
 * @return number of lines occupied by text
 */
int display_text(const char* text, Char* text_Char, size_t len, uint16_t x_position, uint16_t y_position);

/**
 * Converts an integer number to text and displays it
 * @param integer: number to convert to text and to display
 * @param x_position: x position where start writing the text
 * @param y_position: y position where start writing the text
 */
void display_integer(int integer, uint16_t x, uint16_t y);

/**
 * Converts a float number to text and displays it
 * @param decimal: number to convert to text and to display
 * @param x_position: x position where start writing the text
 * @param y_position: y position where start writing the text
 */
void display_float(float decimal, uint16_t x, uint16_t y);
/**
 * Converts seconds to text and display in format min:sec
 * @param seconds: number of seconds 
 * @param x_position: x position where start writing the text
 * @param y_position: y position where start writing the text
 */
void display_time(uint16_t seconds, uint16_t x, uint16_t y);
/**
 * @param c: Char to be displayed
 */ 
void display_Char(Char* c);


#endif
