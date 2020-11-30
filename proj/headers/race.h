#ifndef _LCOM_RACE_H
#define _LCOM_RACE_H

#include <lcom/lcf.h>


typedef struct {
    uint8_t index;
    uint16_t posx;
    uint16_t posy;
} Char;

void race_init(char *text, size_t len);

void update_digited_text(uint8_t aux_key, Char * digited_text, size_t *n_keys);

void update_correct_keys(Char* digited_text, size_t *n_keys, Char* text_int, size_t *correct_keys);



#endif
