#ifndef _LCOM_MENUS_H
#define _LCOM_MENUS_H

#include <lcom/lcf.h>

#include "../xpm/letters.h"
#include "../xpm/menu.h"
#include "../xpm/background.h"
#include "../xpm/others.h"
#include "../headers/mouse.h"
#include "../headers/graphic.h"
#include "../headers/keyboard.h"
#include "../headers/timer.h"
#include "../headers/sprite.h"



typedef enum
{
    MENU,
    RACE,
    RACE_WITH_FRIEND,
    BEST_RESULTS,
    RESULTS,
    EXIT
} Menu_state;

typedef enum
{
    click_on_race,
    click_on_race_with_friend,
    click_on_best_results,
    click_on_exit,
    click_on_try_again_race,
    click_on_try_again_friends_race,
    click_on_save_results,
    type_ESC,
    type_down_arrow,
    type_top_arrow,
    type_right_arrow,
    type_left_arrow, 
    none
    
} Menu_event;

/* "Race" option (x,y) positions */
#define race_x_left  27
#define race_x_right 173
#define race_y_top   268
#define race_y_down  328

/* "Race with friend" option (x,y) positions */
#define friend_race_x_left  303
#define friend_race_x_right 496
#define friend_race_y_top   448
#define friend_race_y_down  532

/* "Best results" option (x,y) positions */
#define best_results_x_left  309
#define best_results_x_right 488
#define best_results_y_top   63
#define best_results_y_down  141

/* "Exit" option (x,y) positions */
#define exit_x_left  625
#define exit_x_right 771
#define exit_y_top   267
#define exit_y_down  329


/*
 * Main menu
 * @return int: 0 if exits with success; 1 if error
 */
int main_menu(Menu_state *state);

/*
 * Displays image for the main menu
 */
void display_main_menu();

/*
 * Draws a white rectangle with a black border
*/
void graphic_draw_bordered_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height);


/*
 * Displays image for the race background
 */
void display_race_background();

/*
 * events:
 *  - click_on_race
 *  - click_on_race_with_friend
 *  - click_on_best_results
 *  - click_on_exit
 *  - click_on_try_again_race
 *  - click_on_try_again_friends_race
 *  - click_on_save_results
 *  - type_ESC 
 *  - type_down_arrow
 *  - type_top_arrow
 *  - type_right_arrow
 *  - type_left_arrow
 *  - none
 * 
 * recebe o pacote e retorna o evento correspondente. 
 * If e else, em que se retorna cada evento caso se clique no botão correspondente (right button num intervalos de posições correctas)
 * subscrever keyboard e mouse
*/
Menu_event read_kbd_event(uint8_t aux_key);
Menu_event read_mouse_event(Mouse_event *ev, int32_t *mouse_x, int32_t *mouse_y);


#endif /* _LCOM_MENUS_H */
