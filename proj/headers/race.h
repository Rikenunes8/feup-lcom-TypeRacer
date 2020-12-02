#ifndef _LCOM_RACE_H
#define _LCOM_RACE_H

#include <lcom/lcf.h>

// Represent a drawn char
typedef struct {
    uint8_t index;
    uint16_t posx;
    uint16_t posy;
} Char;

void race_init(char *text, size_t len);

void update_typed_text(uint8_t aux_key, Char * typed_text, size_t *n_keys);

void update_correct_keys(Char* typed_text, size_t *n_keys, Char* text_Char, size_t *correct_keys);



#endif
