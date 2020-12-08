#ifndef _LCOM_MOUSE_H
#define _LCOM_MOUSE_H

#include <lcom/lcf.h>
#include <i8042.h>

typedef enum {INIT, DRAW1, VERTEX, DRAW2, FINAL} State;
typedef enum {LB_DOWN, LB_UP, MB_DOWN, MB_UP, RB_DOWN, RB_UP, MANY_DOWN, MOVE} Mouse_event;

int (kbc_subscribe_int)(uint8_t *bit_no, uint8_t irq);

int (kbc_unsubscribe_int)(); 

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

/*
 * Por implementar
 * events:
 *  - click_on_race
 *  - click_on_race_with_friend
 *  - click_on_best_results
 *  - click_on_exit
 *  - click_on_try_again_race
 *  - click_on_try_again_friends_race
 *  - click_on_save_results
 *  - type_ESC 
 * 
 * recebe o pacote e retorna o evento correspondente. 
 * If e else, em que se retorna cada evento caso se clique no botão correspondente (right button num intervalos de posições correctas)
 * subscrever keyboard e mouse
*/
Menu_event* read_event(struct packet *pp):
#endif /* _LCOM_MOUSE_H */
