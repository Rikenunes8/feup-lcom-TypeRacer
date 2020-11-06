#ifndef _LCOM_MOUSE_H
#define _LCOM_MOUSE_H

#include <lcom/lcf.h>
#include <i8042.h>

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)(); 

/**
 *  Handles mouse interrupts (C implementation)

    Reads the status register and the output buffer (OB).
    # Reads the scancode byte from the output buffer.
    If there was some error, the byte read from the OB should be discarded.

    All communication with other code must be done via global variables, static if possible.    
 */
void (mouse_ih)();

#endif /* _LCOM_MOUSE_H */