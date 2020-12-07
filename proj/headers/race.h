#ifndef _LCOM_RACE_H
#define _LCOM_RACE_H

#include <lcom/lcf.h>
#include "../headers/graphic.h"
#include "../headers/keyboard.h"


/* (x, y) coordinates for starting positions of the displayed texts*/
#define X_TYPE  20           //x position of typed text
#define Y_TYPE  200          //y position of typed text
#define X_TEXT  20           //x position of displayed text
#define Y_TEXT  100          //y position of displayed text
#define X_TIME  20           //x position of displayed text: "Total time:"
#define Y_TIME  400          //y position of displayed text: "Total time:"
#define X_TIME_RESULT  160   //x position of displayed text: no_seconds
#define Y_TIME_RESULT  400   //y position of displayed text: no_seconds
#define X_CPM_INTRO  20      //x position of displayed text: "Your speed:"
#define Y_CPM_INTRO  450     //y position of displayed text: "Your speed:"
#define X_CPM_RESULT  160    //x position of displayed text: CPM
#define Y_CPM_RESULT  450    //y position of displayed text: CPM
#define X_CPM  210           //x position of displayed text: "caracters per minute"
#define Y_CPM  450           //y position of displayed text: "caracters per minute"

/*
 * @param text: text to draw
 * @param len: lenght of that text
 */
void race_init(const char *text, size_t len);

/* Processes the typed text and writes it to the screen 
 * @param aux_key: number corresponding to a scancode of the typed key 
 * @param typed_text: array with the typed chars, representing the text typed by the user
 * @param n_keys: lenght of typed_text
 * @param current_key: 
 */
void update_typed_text(uint8_t aux_key, Char * typed_text, size_t *n_keys, size_t *current_key);

/* Compares the typed text with the displayed text and counts the correct chars typed
 * @param typed_text: array with typed chars
 * @param n_keys: lenght of typed_text
 * @param text_Char: array with chars displayed on the screen and that the user is suppose to type
 * @param correct_keys: counter that counts the correct keys typed
 */
void update_correct_keys(Char* typed_text, size_t *n_keys, Char* text_Char, size_t *correct_keys, size_t *len);

/*
 * Displays text defined
 * @param text: text to display on the screen
 * @param text_Char: place to where convert text in order to be drawable 
 * @param len: lenght of text
 * @param x_position: x position where start writing the text
 * @param y_position: y position where start writing the text
 */
int display_text(const char* text, Char* text_Char, size_t len, uint16_t x_position, uint16_t y_position);

/*
 * Converts an integer number to text and displays it
 * @param integer: number to convert to text and to display
 * @param x_position: x position where start writing the text
 * @param y_position: y position where start writing the text
 */
void display_integer(int integer, uint16_t x, uint16_t y);

/*
 * Converts a float number to text and displays it
 * @param decimal: number to convert to text and to display
 * @param x_position: x position where start writing the text
 * @param y_position: y position where start writing the text
 */
void display_float(float decimal, uint16_t x, uint16_t y);

void display_Char(Char *c);

/*
 * Displays the results
 * @param no_minutes: number of minutes
 * @param no_seconds: number of seconds
 * @param correct_keys: number of correct keys typed
 * @param count_backspaces: number of backspaces "typed"
 * @param len: lenght of the displayed text
 */
void display_results(size_t no_minutes, size_t no_seconds, size_t correct_keys, size_t count_backspaces, size_t n_keys, size_t len);

void rearrange_coors_text(Char* typed_text, size_t begin, size_t end);




#endif
