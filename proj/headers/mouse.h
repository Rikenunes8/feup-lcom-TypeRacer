#ifndef _LCOM_MOUSE_H
#define _LCOM_MOUSE_H

#include <lcom/lcf.h>
#include <../headers/i8042.h>

typedef enum {LB_DOWN, LB_UP, MB_DOWN, MB_UP, RB_DOWN, RB_UP, MANY_DOWN, MOVE} Mouse_event_t;

typedef struct {
  Mouse_event_t ev;
  int32_t dx; int32_t dy;
} Mouse_event;

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

int (kbc_write_byte)(uint8_t cmd, uint8_t arg);

void assemble_packet(struct packet *pp);

void mouse_events(Mouse_event *event, struct packet *pp);

#endif /* _LCOM_MOUSE_H */
