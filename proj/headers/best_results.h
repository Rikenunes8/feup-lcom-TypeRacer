#ifndef _LCOM_BEST_RESULTS_H
#define _LCOM_BEST_RESULTS_H

#include "../headers/sprite.h"
#include "../headers/menus.h"

/** @defgroup best_results Best results
 * @{
 *
 * Best results related functions, variables and data structures
 */


/** 
 * @brief Struct that keeps the game's results
 */
typedef struct 
{
    size_t cpm; /*!< Caracters per minute */
    float accuracy; /*!< Accuracy in percentage */
    uint8_t* date; /*!< Date and time when the game was played */
    char name[20]; /*!< Name choosen by the player */
} Score;


/** 
 * @brief Reads the information of the file best_results.txt 
 * where are kept the 3 best scores obtained in the game.
 * Put the information read in Score* best_scores. 
 * @return 0 if success (ainda não verifica erros)
 */
int br_read_file();

/** 
 * @brief Writes the information of the 3 best scores obtained in the game ever 
 * into the file best_results.txt 
 * @return 0 if success (ainda não verifica erros)
 */
int br_write_file();

/** 
 * @brief Draw the components of the table (titles and results)
 * @return 0 if success
 */
int br_draw_best_results();

/** 
 * @brief Draw best results page and copy to an auxiliary buffer
 */
void br_init();

/** 
 * @brief Free auxiliary buffer and background
 */
void br_end();

/**
 * @brief Process timer interrupt in best results page
 * @details Draw auxiliary buffer and mouse 
 * @param counter Timer count
 * @param mouse Mouse sprite
 */
void br_process_timer_int(uint32_t counter, Sprite* mouse);

/**
 * @brief Process keyboard interrupt in best results page
 * @details Check if it is to leave the page and change state if so
 * @param state Current state of state machine
 * @param aux_key Key to process
 */
void br_process_kbd_int(Menu_state *state, uint8_t aux_key);

/**
 * @brief Process mouse interrupt in best results page
 * @details Check if it is to leave the page and change state if so. Move mouse.
 * @param state Current state of state machine
 * @param mouse_event Mouse event
 * @param mouse Mouse sprite
 */
void br_process_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse);

/**
 * @brief Compare new score with current best scores and replace if the case. Get the current date from RTC
 * 
 * @param CPM Characters per minute of new score
 * @param accuracy Accuracy of new score
 * @param name Name of new score
 * @return int 0 if success
 */
int add_score(size_t CPM, float accuracy, char* name);



#endif
