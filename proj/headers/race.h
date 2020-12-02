#ifndef _LCOM_RACE_H
#define _LCOM_RACE_H

#include <lcom/lcf.h>
#include "../headers/graphic.h"
#include "../headers/keyboard.h"

enum Char_state {
  NORMAL,
  WRONG,
  RIGHT
};
/* Represents a drawn char
 * index: number corresponding to a char
 * posx: x position where the char is drawn
 * posy: y position where the char is drawn 
 */
typedef struct {
    uint8_t index;
    uint16_t posx;
    uint16_t posy;
    //enum Char_state state;
} Char;

/*
 * @param text: text to draw
 * @param len: lenght of that text
 */
void race_init(const char *text, size_t len);

/* Processes the typed text and writes it to the screen 
 * @param aux_key: number corresponding to a scancode of the typed key 
 * @param typed_text: array with the typed chars, representing the text typed by the user
 * @param n_keys: lenght of typed_text
 */
void update_typed_text(uint8_t aux_key, Char * typed_text, size_t *n_keys);

/* Compares the typed text with the displayed text and counts the correct chars typed
 * @param typed_text: array with typed chars
 * @param n_keys: lenght of typed_text
 * @param text_Char: array with chars displayed on the screen and that the user is suppose to type
 * @param correct_keys: counter that counts the correct keys typed
 */
void update_correct_keys(Char* typed_text, size_t *n_keys, Char* text_Char, size_t *correct_keys);

/*
 * @param text: text to display on the screen
 * @param text_Char: place to where convert text in order to be drawable 
 * @param len: lenght of text
 * @param x_position: x position where start writing the text
 * @param y_position: y position where start writing the text
 */
int display_text(const char* text, Char* text_Char, size_t len, uint16_t x_position, uint16_t y_position);

void display_time(int time, uint16_t x, uint16_t y);



#endif
