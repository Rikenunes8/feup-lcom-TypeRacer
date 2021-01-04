#ifndef _LCOM_RACE_H
#define _LCOM_RACE_H

#include "../headers/mouse.h"
#include "../headers/Chars.h"
#include "../headers/menus.h"

/** @defgroup race race
 * @{
 *
 * Race page related functions, variables and data structures
 */


/* (x, y) coordinates for starting positions of the displayed texts*/
#define X_TYPE          50   //!< x starting position of typed text
uint16_t y_pos_typed;        //!< y starting position of typed text         
#define X_TEXT          48   //!< x starting position of displayed text
#define Y_TEXT          170  //!< y starting position of displayed text
#define X_BOX           30   //!< x starting position of first text box
#define Y_BOX           160  //!< y starting position of first text box
#define Y_BOX_MARGIN    10   //!< Y_BOX - Y_TEXT
#define BOX_WIDTH       740  //!< Text box width
#define Y_BTW_BOXES     20   //!< Height between text boxes



/**
 * @brief Initializes race
 * @details Allocates space to objects need. Resets all variables need. Sets the alarm
 * 
 * @param text Text to play with
 * @param len Length of the text
 */
void race_init(const char *text, size_t len);

/**
 * @brief Ends race
 * @details Frees memory allocated in race init and destroy objects
 * 
 */
void race_end();
/**
 * @brief Processes timer interrupt in race page
 * @details Manages the traffic lights and the beggining of the race.
 * Draw all page with a frame rate of 30. 
 * 
 * @param state Current state of state machine
 * @param counter Timer count
 * @param mouse Mouse sprite
 */
void race_process_timer_int(Menu_state *state, uint32_t counter, Sprite* mouse);
/**
 * @brief Processes keyboard interrupt in race page
 * @details Calls the game logic functions: update_typed_text and update_correct_keys. Deals with giving up
 * @param state Current state of state machine
 * @param aux_key Key to process
 */
void race_process_kbd_int(Menu_state *state, uint8_t aux_key);
/**
 * @brief Processes mouse interrupt in race page
 * @details Moves mouse and changes the position of text's cursor if there is a mouse click above a typed Char
 * @param state Current state of state machine
 * @param mouse_event Mouse event
 * @param mouse Mouse sprite
 */
void race_process_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse);

/**
 * @brief Processes the typed text and writes it to the screen 
 * @details Processes all the keyboard inputs to write and erase characters, and to move the cursor to left or right
 * @param aux_key Key to process
 */
void update_typed_text(uint8_t aux_key);


/**
 * @brief Compares the typed text with the displayed text and counts the correct chars typed
 * @details Checks if Chars of typed text are the same of text to type, sets correct keys and draws text to type with new colors
 */
void update_correct_keys();
/**
 * @brief Rearranges coorinates of text of Chars
 *  
 * @param typed_text Text of Chars to set new coordinates
 * @param begin Index of Char where to start setting new coordinates
 * @param end Last coordinates set
 */
void rearrange_coors_text(Char* typed_text, size_t begin, size_t end);

/**
 * @brief Draws a white rectangle with a black border
 * 
 * @param x X position of rectangle top-left corner
 * @param y y position of rectangle top-left corner
 * @param width Width of the rectangle
 * @param height Height of the rectangle
 */
void graphic_draw_bordered_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief Draws a text box
 * 
 * @param x X position of text box top-left corner
 * @param y y position of text box top-left corner
 * @param width Width of the text box
 */
void draw_text_box(uint16_t x, uint16_t y, uint16_t width);
/**
 * @brief Displays race background
 * 
 */
void display_race_background();

/** @} end of race */

/** @defgroup results results
 * @{
 *
 * Results page related functions, variables and data structures
 */

/**
 * @brief Displays the results
 * 
 * @param real_time True to display results in race page, false to display results in results page
 */
void display_results(bool real_time);

/**
 * @brief Sets the CPM and accuracy
 * 
 */
void set_results();



/**
 * @brief Initializes results page and allocates memory
 * 
 */
void results_init();
/**
 * @brief Frees alocated memory in results_init and calls add_score function
 * 
 */
void results_end();
/**
 * @brief Processes timer interrupt in results page
 * @details Copies auxiliary buffer to frame buffer, draws name, draws bubbles and draws mouse
 * 
 * @param counter Timer count
 * @param mouse Mouse sprite
 */
void results_process_timer_int(uint32_t counter, Sprite* mouse);
/**
 * @brief Processes keyboard interrupt in results page
 * @details Checks if it is to leave results page and handles name writing
 * @param state Current state of state machine
 * @param aux_key Key to process
 */
void results_process_kbd_int(Menu_state *state, uint8_t aux_key);
/**
 * @brief Processes mouse interrupt in race page
 * @details Checks if it is to leave results page, moves mouse and 
 * checks colisions of the mouse with the bubbles by clicking on it
 * 
 * @param state Current state of state machine
 * @param mouse_event Mouse event
 * @param mouse Mouse sprite
 */
void results_process_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse);
/**
 * @brief Checks if mouse collided with some bubble and prepares bubble to be destroyed if it is the case
 * 
 * @param mouse Mouse sprite
 * @return True in case of any collision, false otherwise
 */
bool collison_mouse(Sprite* mouse);
/**
 * @brief Removes bubble to be erased of the array of bubbles and destroys it
 * 
 * @param n Index of the bubble to be destroyed in array of bubbles
 */
void bubbles_erase(size_t n);
/**
 * @brief Moves bubbles
 * @details Checks collisions with screen bounderies and changes bubble speed directions.
 * Animates bubble. Erases bubble if it is the case
 * 
 * @param n Index of the bubble to me moved in array of bubbles
 */
void move_bubbles(size_t n);

/** @} end of results */

#endif
