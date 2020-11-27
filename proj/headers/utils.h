#ifndef _H_UTILS_
#define _H_UTILS_

#include <lcom/lcf.h>
#include <stdint.h>

#define OK 0

int subscribe_int(uint8_t *bit_no, uint8_t irq, int policy);
int unsubscribe_int();

#endif
