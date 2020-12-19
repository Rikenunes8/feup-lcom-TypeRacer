#ifndef _LCOM_RACE_H
#define _LCOM_RACE_H

#include <lcom/lcf.h>

#include "../xpm/letters.h"
#include "../xpm/others.h"
#include "../xpm/bubbles.h"
#include "../xpm/background.h"
#include "../xpm/results_page.h"
#include "../headers/graphic.h"
#include "../headers/menus.h"
#include "../headers/keyboard.h"



/* (x, y) coordinates for starting positions of the displayed texts*/
#define X_TYPE          50           //x position of typed text
//#define Y_TYPE        426          //y position of typed text
#define X_TEXT          50           //x position of displayed text
#define Y_TEXT          170          //y position of displayed text
#define X_BOX           32
#define Y_BOX           160
#define Y_BOX_MARGIN    10          // Y_BOX - Y_TEXT
#define BOX_WIDTH       736
#define Y_BTW_BOXES     20          // height between text boxes

#define MAX_NO_LINES 8 //maximum number of lines that the text can have

extern size_t no_lines;
extern uint16_t y_pos_typed;


/*
 * @param text: text to draw
 * @param len: lenght of that text
 */
void race_init(const char *text, size_t len, size_t n_lines);

void race_end();


void race_process_timer_int(uint32_t counter);

void race_process_kbd_int(Menu_state *state, uint8_t aux_key);

void race_process_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse);


void results_init();

void results_end();

void results_proccess_timer_int(uint32_t counter, Sprite* mouse);

void results_proccess_kbd_int(Menu_state *state, uint8_t aux_key);

void results_proccess_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse);


/* Processes the typed text and writes it to the screen 
 * @param aux_key: number corresponding to a scancode of the typed key 
 * @param typed_text: array with the typed chars, representing the text typed by the user
 * @param n_keys: lenght of typed_text
 * @param current_key: 
 */
void update_typed_text(uint8_t aux_key);

/* Compares the typed text with the displayed text and counts the correct chars typed
 * @param typed_text: array with typed chars
 * @param n_keys: lenght of typed_text
 * @param text_Char: array with chars displayed on the screen and that the user is suppose to type
 * @param correct_keys: counter that counts the correct keys typed
 */
void update_correct_keys();

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

void display_time(uint16_t seconds, uint16_t x, uint16_t y);

void display_Char(Char* c);

/*
 * Displays the results
 * @param no_minutes: number of minutes
 * @param no_seconds: number of seconds
 * @param correct_keys: number of correct keys typed
 * @param count_backspaces: number of backspaces "typed"
 * @param len: lenght of the displayed text
 */
void display_results(size_t no_seconds, size_t correct_keys, size_t count_backspaces, size_t n_keys, size_t len, bool real_time);

void rearrange_coors_text(Char* typed_text, size_t begin, size_t end);


void graphic_draw_bordered_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

void draw_text_box(uint16_t x, uint16_t y, uint16_t width, size_t no_lines);

void display_race_background(size_t no_lines);


void collison_mouse(Sprite* mouse);

void bubbles_erase(size_t n);

void move_bubbles(size_t n);



#endif
