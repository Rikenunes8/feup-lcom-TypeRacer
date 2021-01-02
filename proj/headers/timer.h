#ifndef _LCOM_TIMER_H
#define _LCOM_TIMER_H


#include "../headers/i8254.h"

/** @defgroup timer
 * @{
 *
 * Timer related functions, variables and data structures
 */


/**
 * @brief Sets frequency of timer
 * 
 * @param timer Timer (0, 1 or 2)
 * @param freq New frequency
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);
/**
 * @brief Subscribes and enables Keyboard interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (timer_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribes Timer interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (timer_unsubscribe_int)();
/**
 * @brief Handles timer interrupts (C implementation)
 * @details Increases by one the timer counter.
 *  All communication with other code must be done via global variables, static if possible.    
 */
void (timer_ih)();
/**
 * @brief Gets configuration of timer
 * 
 * @param timer Timer (0, 1 or 2)
 * @param st Status byte to be set
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);
/**
 * @brief Displays configuration of timer in a human friendly way
 * 
 * @param timer Timer (0, 1 or 2)
 * @param st Status byte
 * @param field Structure with timer info
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

/** @} end of timer */
#endif 
