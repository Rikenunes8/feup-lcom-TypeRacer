#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <../headers/i8042.h>
/** @defgroup keyboard keyboard
 * @{
 *
 * Keyboard related functions, variables and data structures
 */


/**
 * @brief Subscribes and enables Keyboard interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes Keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (kbd_unsubscribe_int)(); 

/**
 * @brief Handles keyboard interrupts (C implementation)
 *  @details Reads the status register and the output buffer (OB).
 *  Reads the scancode byte from the output buffer.
 *  If there was some error, the byte read from the OB should be discarded.
 *  All communication with other code must be done via global variables, static if possible.    
 */
void (kbc_ih)();
/**
 * @brief Reads the command byte of KBC i8042
 * 
 * @param cmd Command byte to be set
 * @return Return 0 upon success and non-zero otherwise
 */
int read_cmd_byte(uint8_t *cmd);
/**
 * @brief Writes the command byte of KBC i8042
 * 
 * @param cmd Command byte to write
 * @return Return 0 upon success and non-zero otherwise
 */
int write_cmd_byte(uint8_t *cmd);
/**
 * @brief Assembles the scancode byte(s) in an array of 2 bytes
 * 
 * @param bytes Complete scancode to be set
 */
void assemble_scancode(uint8_t *bytes);
/**
 * @brief Prints scancode in human friendly way
 * 
 * @param bytes Complete scancode to print
 */
void print_scancode(uint8_t *bytes);

/** @} end of keyboard */

#endif /* _LCOM_KEYBOARD_H */
