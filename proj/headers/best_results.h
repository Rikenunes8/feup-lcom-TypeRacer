#ifndef _LCOM_BEST_RESULTS_H
#define _LCOM_BEST_RESULTS_H

#include <lcom/lcf.h>

#include "../headers/menus.h"
#include "../headers/Chars.h"


typedef struct {
    size_t cpm;
    float accuracy;
    uint8_t* date;
    char name[12];
} Score;

int br_read_file();

int br_write_file();

int br_draw_best_results();

void br_init();

void br_end();

void br_process_timer_int(uint32_t counter, Sprite* mouse);

void br_process_kbd_int(Menu_state *state, uint8_t aux_key);

void br_process_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse);

int add_score(size_t CPM, float accuracy, char* name);



#endif
