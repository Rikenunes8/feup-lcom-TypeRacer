#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_


#include <lcom/lcf.h>
#include <../headers/i8042.h>

int (kbc_subscribe_int)(uint8_t *bit_no);

int (kbc_unsubscribe_int)(); 

/**
 *  Handles keyboard interrupts (C implementation)

    Reads the status register and the output buffer (OB).
    Reads the scancode byte from the output buffer.
    If there was some error, the byte read from the OB should be discarded.

    All communication with other code must be done via global variables, static if possible.    
 */
void (kbc_ih)();

int read_cmd_byte(uint8_t *cmd);

int write_cmd_byte(uint8_t *cmd);

void assemble_scancode(uint8_t *bytes);

void print_scancode(uint8_t *bytes);

uint8_t get_scancode_char(uint8_t *bytes);

#endif /* _LCOM_KEYBOARD_H */
