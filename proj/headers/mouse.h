#ifndef _LCOM_MOUSE_H
#define _LCOM_MOUSE_H

#include <lcom/lcf.h>
#include <../headers/i8042.h>
/**
 * @brief Mouse events by pressing, releasing and move
 * 
 */
typedef enum {LB_DOWN, LB_UP, MB_DOWN, MB_UP, RB_DOWN, RB_UP, MANY_DOWN, MOVE} Mouse_event_t;

/**
 * @brief Represent the info of a mouse packet
 * 
 */
typedef struct {
  Mouse_event_t ev;   /** Mouse event */
  int32_t dx;         /** X displacement */
  int32_t dy;         /** Y displacement */
} Mouse_event;

/**
 * @brief Subscribes and enables Mouse interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribes Mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (mouse_unsubscribe_int)(); 

/**
 * @brief Handles mouse interrupts (C implementation)
 * @details Reads the status register and the output buffer (OB).
 * Reads one packet byte from the output buffer.
 * If there was some error, the byte read from the OB should be discarded.
 * All communication with other code must be done via global variables, static if possible.
 */
void (mouse_ih)();

/**
 * @brief Writes a command byte and the argument (if the case) to the kbc i8042
 * 
 * @param cmd Command byte to write
 * @param arg Argument to write
 * @return Return 0 upon success and non-zero otherwise
 */
int kbc_write_byte(uint8_t cmd, uint8_t arg);
/**
 * @brief Assemble the 3 packets bytes of the mouse
 * 
 * @param pp Structure to be set with mouse packet bytes information
 */
void assemble_packet(struct packet *pp);
/**
 * @brief Convert mouse info to a more friendly structure
 * 
 * @param event New mouse info
 * @param pp Packet mouse info
 */
void mouse_events(Mouse_event *event, struct packet *pp);

#endif /* _LCOM_MOUSE_H */
