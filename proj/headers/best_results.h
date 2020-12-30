#ifndef _LCOM_BEST_RESULTS_H
#define _LCOM_BEST_RESULTS_H

#include "../headers/graphic.h"
#include "../xpm/best_results_page.h"
#include "../headers/rtc.h"
#include "../headers/menus.h"
#include "../headers/Chars.h"

/** @defgroup best_results Best results
 * @{
 *
 * Best results related functions, variables and data structures
 */


/** Struct that keeps the game's results
 */
typedef struct 
{
    size_t cpm; /*!< Caracters per minute */
    float accuracy; /*!< Accuracy in percentage */
    uint8_t* date; /*!< Date and time when the game was played */
    char name[20]; /*!< Name choosen by the player */
} Score;


/** Reads the information of the file best_results.txt 
 * where we keep the 3 best scores obtained in the game.
 * @return 0 if success (ainda não verifica erros)
 */
int br_read_file();

/** Writes the information of the 3 best scores obtained in the game ever
 * into the file best_results.txt 
 * @return 0 if success (ainda não verifica erros)
 */
int br_write_file();

/** Falta
 * @return 
 */
int br_draw_best_results();

/** Falta
 * @return 
 */
void br_init();

/** Falta
 * @return 
 */
void br_end();

/** Falta
 * @param
 * @return 
 */
void br_process_timer_int(uint32_t counter, Sprite* mouse);

/** Falta
 * @param
 * @return 
 */
void br_process_kbd_int(Menu_state *state, uint8_t aux_key);

/** Falta
 * @param
 * @return 
 */
void br_process_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse);

/** Falta
 * @param
 * @return 
 */
int add_score(size_t CPM, float accuracy, char* name);



#endif
