#ifndef _LCOM_MOUSE_H
#define _LCOM_MOUSE_H

#include <lcom/lcf.h>

typedef enum
{
    MAIN,
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
    type_ESC
    
} Menu_event;

/*
 * Main menu
 * @return int: 0 if exits with success; 1 if error
 */
int main_menu();

#endif /* _LCOM_MOUSE_H */