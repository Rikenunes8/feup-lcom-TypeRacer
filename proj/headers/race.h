#ifndef _LCOM_RACE_H
#define _LCOM_RACE_H

#include "../headers/mouse.h"
#include "../headers/Chars.h"
#include "../headers/menus.h"



/* (x, y) coordinates for starting positions of the displayed texts*/
#define X_TYPE          50          //!x position of typed text
uint16_t y_pos_typed;               //!y position of typed text
//#define Y_TYPE        426          
#define X_TEXT          50          //!x position of displayed text
#define Y_TEXT          170         //!y position of displayed text
#define X_BOX           31          //!x position of first text box
#define Y_BOX           160         //!y position of first text box
#define Y_BOX_MARGIN    10          //! Y_BOX - Y_TEXT
#define BOX_WIDTH       738
#define Y_BTW_BOXES     20          //! Height between text boxes

#define MAX_NO_LINES 8 //maximum number of lines that the text can have



/**
 * @brief Initialize race
 * @details Allocate space to objects need. Reset all variables need. Set the alarm
 * 
 * @param text Text to play with
 * @param len Length of the text
 */
void race_init(const char *text, size_t len);
/**
 * @brief End race
 * @details Free memory allocated in race init and destroy objects
 * 
 */
void race_end();
/**
 * @brief Process timer interrupt in race page
 * @details
 * 
 * @param state Current state of state machine
 * @param counter Timer count
 * @param mouse Mouse sprite
 */
void race_process_timer_int(Menu_state *state, uint32_t counter, Sprite* mouse);
/**
 * @brief Process keyboard interrupt in race page
 * @details 
 * @param state Current state of state machine
 * @param aux_key Key to process
 */
void race_process_kbd_int(Menu_state *state, uint8_t aux_key);
/**
 * @brief Process mouse interrupt in race page
 * @details Move mouse
 * @param state Current state of state machine
 * @param mouse_event Mouse event
 * @param mouse Mouse sprite
 */
void race_process_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse);

/**
 * @brief Processes the typed text and writes it to the screen 
 * @details Process all the keyboard inputs to write and erase characters, and to move the cursor to left or right
 * @param aux_key Key to process
 */
void update_typed_text(uint8_t aux_key);


/**
 * @brief Compares the typed text with the displayed text and counts the correct chars typed
 * @details Check if Chars of typed text are the same of text to type, set correct keys and draw text to type with new colors
 */
void update_correct_keys();
/**
 * @brief Rearrange coorinates of text of Chars
 *  
 * @param typed_text Text of Chars to set new coordinates
 * @param begin Index of Char where to start setting new coordinates
 * @param end Last coordinates set
 */
void rearrange_coors_text(Char* typed_text, size_t begin, size_t end);

/**
 * @brief Draw a rectangle white with a black border
 * 
 * @param x X position of rectangle top-left corner
 * @param y y position of rectangle top-left corner
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 */
void graphic_draw_bordered_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief Draw a text box
 * 
 * @param x X position of text box top-left corner
 * @param y y position of text box top-left corner
 * @param width Width of the text box
 */
void draw_text_box(uint16_t x, uint16_t y, uint16_t width);
/**
 * @brief Display race background
 * 
 */
void display_race_background();

/**
 * @brief Display the results
 * 
 * @param real_time True to display results in race page, false to display results in results page
 */
void display_results(bool real_time);

/**
 * @brief Set the CPM and accuracy
 * 
 */
void set_results();





/**
 * @brief Initialize results page and allocate memory
 * 
 */
void results_init();
/**
 * @brief Free alocated memory in results_init and call add_score
 * 
 */
void results_end();
/**
 * @brief Process timer interrupt in results page
 * @details Copy auxiliary buffer to frame buffer, draw name, draw bubbles and draw mouse
 * 
 * @param counter Timer count
 * @param mouse Mouse sprite
 */
void results_process_timer_int(uint32_t counter, Sprite* mouse);
/**
 * @brief Process keyboard interrupt in results page
 * @details Check if it is to leave results page and handle name writing
 * @param state Current state of state machine
 * @param aux_key Key to process
 */
void results_process_kbd_int(Menu_state *state, uint8_t aux_key);
/**
 * @brief Process mouse interrupt in race page
 * @details Check if it is to leave results page, move mouse and 
 * check colisions of the mouse with the bubbles by clicking on it
 * 
 * @param state Current state of state machine
 * @param mouse_event Mouse event
 * @param mouse Mouse sprite
 */
void results_process_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse);
/**
 * @brief Check if mouse collided with some bubble and prepare bubble to be destroyed if it is the case
 * 
 * @param mouse Mouse sprite
 * @return True in case of any collision, false otherwise
 */
bool collison_mouse(Sprite* mouse);
/**
 * @brief Remove bubble to be erased of the array of bubbles and destroy it
 * 
 * @param n Index of the bubble to be destroyed in array of bubbles
 */
void bubbles_erase(size_t n);
/**
 * @brief Move bubbles
 * @details Check collisions with screen bounderies and change bubble speed directions.
 * Animate bubble. Erase bubble if it is the case
 * 
 * @param n Index of the bubble to me moved in array of bubbles
 */
void move_bubbles(size_t n);



#endif
