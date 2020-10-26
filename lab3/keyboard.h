#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_


#include <lcom/lcf.h>
#include <i8042.h>

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

int (read_cmd_byte(uint32_t *cmd));

int (write_cmd_byte(uint32_t *cmd));

void (assembleScancode(uint8_t *bytes, size_t size));


#endif /* _LCOM_KEYBOARD_H */
