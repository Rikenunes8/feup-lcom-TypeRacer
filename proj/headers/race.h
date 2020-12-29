#ifndef _LCOM_RACE_H
#define _LCOM_RACE_H

#include <lcom/lcf.h>

#include "../xpm/letters.h"
#include "../xpm/others.h"
#include "../xpm/bubbles.h"
#include "../xpm/background.h"
#include "../xpm/results_page.h"
#include "../headers/graphic.h"
#include "../headers/Chars.h"
#include "../headers/menus.h"
#include "../headers/keyboard.h"



/* (x, y) coordinates for starting positions of the displayed texts*/
#define X_TYPE          50           //x position of typed text
uint16_t y_pos_typed;                //y position of typed text
//#define Y_TYPE        426          
#define X_TEXT          50           //x position of displayed text
#define Y_TEXT          170          //y position of displayed text
#define X_BOX           31
#define Y_BOX           160
#define Y_BOX_MARGIN    10          // Y_BOX - Y_TEXT
#define BOX_WIDTH       738
#define Y_BTW_BOXES     20          // height between text boxes

#define MAX_NO_LINES 8 //maximum number of lines that the text can have




/**
 * @param text: text to draw
 * @param len: lenght of that text
 */
void race_init(const char *text, size_t len);

void race_end();

void race_process_timer_int(uint32_t counter, Sprite* mouse);

void race_process_kbd_int(Menu_state *state, uint8_t aux_key);

void race_process_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse);


/**
 * Processes the typed text and writes it to the screen 
 * @param aux_key: number corresponding to a scancode of the typed key
 */
void update_typed_text(uint8_t aux_key);

/**
 * Compares the typed text with the displayed text and counts the correct chars typed
 */
void update_correct_keys();
/**
 * @param
 * @param
 * @param
 */
void rearrange_coors_text(Char* typed_text, size_t begin, size_t end);

/**
 * @param
 * @param
 * @param
 * @param
 */
void graphic_draw_bordered_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
/**
 * @param
 * @param
 * @param
 */
void draw_text_box(uint16_t x, uint16_t y, uint16_t width);

void display_race_background();

/**
 * Displays the results
 * @param real_time: true to display results in race page, false to display results in results page
 */
void display_results(bool real_time);
/**
 * Set cpms and accuracy
 */
void set_results();






void results_init();

void results_end();

void results_proccess_timer_int(uint32_t counter, Sprite* mouse);

void results_proccess_kbd_int(Menu_state *state, uint8_t aux_key);

void results_proccess_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse);

void collison_mouse(Sprite* mouse);

void bubbles_erase(size_t n);

void move_bubbles(size_t n);



#endif
