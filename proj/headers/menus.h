#ifndef _LCOM_MENUS_H
#define _LCOM_MENUS_H

#include "../headers/mouse.h"
#include "../headers/sprite.h"

/** @defgroup menus menus
 * @{
 *
 * Menus related functions, variables and data structures
 */

/**
 * @brief Enumerator with various states for the game main state machine
 * 
 */
typedef enum
{
    MENU,
    RACE,
    RACE_WITH_FRIEND,
    BEST_RESULTS,
    RESULTS,
    EXIT
} Menu_state;

/**
  @brief Enumerator with various possible keyboard and mouse events
 * 
 */
typedef enum
{
    click_on_race,
    click_on_race_with_friend,
    click_on_best_results,
    click_on_exit,
    click_on_results_exit,
    click_on_try_again_race,
    click_on_try_again_friends_race,
    click_on_best_results_back,
    type_ESC,
    type_down_arrow,
    type_top_arrow,
    type_right_arrow,
    type_left_arrow, 
    none 
} Menu_event;

/* "Race" option (x,y) positions */
#define race_x_left  27  //!< x position for top-left side of "Race" rectangle from main menu
#define race_x_right 173 //!< x position for down-right side of "Race" rectangle from main menu 
#define race_y_top   268 //!< y position for top-left side of "Race" rectangle from main menu
#define race_y_down  328 //!< x position for down-right side of "Race" rectangle from main menu 

/* "Race with friend" option (x,y) positions */
#define friend_race_x_left  303 //!< x position for top-left side of "Race with friend" rectangle from main menu
#define friend_race_x_right 496 //!< x position for down-right side of "Race with friend" rectangle from main menu  
#define friend_race_y_top   448 //!< y position for top-left side of "Race with friend" rectangle from main menu
#define friend_race_y_down  532 //!< y position for down-right side of "Race with friend" rectangle from main menu   

/* "Best results" option (x,y) positions */
#define best_results_x_left  309 //!< x position for top-left side of "Best results" rectangle from main menu
#define best_results_x_right 488 //!< x position for down-right side of "Best results" rectangle from main menu
#define best_results_y_top   63  //!< y position for top-left side of "Best results" rectangle from main menu
#define best_results_y_down  141 //!< y position for down-right side of "Best results" rectangle from main menu

/* "Exit" option (x,y) positions */
#define exit_x_left  625 //!< x position for top-left side of "Exit" rectangle from main menu
#define exit_x_right 771 //!< x position for down-right side of "Exit" rectangle from main menu
#define exit_y_top   267 //!< y position for top-left side of "Exit" rectangle from main menu
#define exit_y_down  329 //!< y position for down-right side of "Exit" rectangle from main menu

/* "Try again" option (x,y) positions */
#define try_again_x_left   250 //!< x position for top-left side of "Try again" rectangle from "Results" page      
#define try_again_x_right  400 //!< x position for down-right side of "Try again" rectangle from "Results" page      
#define try_again_y_top    420 //!< y position for top-left side of "Try again" rectangle from "Results" page    
#define try_again_y_down   470 //!< y position for down-right side of "Try again" rectangle from "Results" page       

/* "Exit" option (x,y) positions from results page*/
#define results_exit_x_left   450     //!< x position for top-left side of "Exit" rectangle from "Results" page
#define results_exit_x_right  550     //!< x position for down_right side of "Exit" rectangle from "Results" page 
#define results_exit_y_top    420     //!< y position for top-left side of "Exit" rectangle from "Results" page
#define results_exit_y_down   470     //!< y position for down_right side of "Exit" rectangle from "Results" page 

/* "Back" option (x,y) positions from best results page*/
#define best_results_back_x_left   685     //!< x position for top-left side of "Back" rectangle from "Best results" page
#define best_results_back_x_right  785     //!< x position for down_right side of "Back" rectangle from "Best results" page
#define best_results_back_y_top    520     //!< y position for top-left side of "Back" rectangle from "Best results" page
#define best_results_back_y_down   570     //!< y position for down_right side of "Back" rectangle from "Best results" page

/**
 * @brief Processes timer interrupt in menu page
 * @details Draw main menu and mouse 
 * @param counter Timer count 
 * @param main_menu Main menu sprite
 * @param mouse Mouse sprite
 */
void menus_process_timer_int(uint32_t counter, Sprite* main_menu, Sprite* mouse);
/**
 * @brief Processes keyboard interrupt in menu page
 * @details Check if it is to go to another page and change state if so
 * @param state Current state of state machine
 * @param aux_key Key to process
 */
void menus_process_kbd_int(Menu_state *state, uint8_t aux_key);
/**
 * @brief Processes mouse interrupt in menu page
 * @details Check if it is to go to another page and change state if so. Move mouse
 * @param state Current state of state machine
 * @param mouse_event Mouse event
 * @param mouse Mouse sprite
 */
void menus_process_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse);


/**
 * @brief Gets Menu event generated by keyboard action
 * 
 * @param aux_key Key to process
 * @return Menu_event 
 */
Menu_event read_kbd_event(uint8_t aux_key);
/**
 * @brief Gets Menu event generated by mouse action
 * 
 * @param ev Mouse event
 * @param mouse_x X position of mouse
 * @param mouse_y Y position of mouse
 * @return Menu_event 
 */
Menu_event read_mouse_event(Mouse_event *ev, int32_t *mouse_x, int32_t *mouse_y);

#endif /* _LCOM_MENUS_H */
