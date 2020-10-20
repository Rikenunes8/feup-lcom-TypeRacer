#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_


#include <lcom/lcf.h>
#include <lcom/i8042.h>


int keyboard_subscribe_int(uint8_t *bit_no);

int keyboard_unsubscribe_int(); 

#endif /* _LCOM_KEYBOARD_H */