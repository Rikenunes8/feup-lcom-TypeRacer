#ifndef _H_PROJ_
#define _H_PROJ_

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

typedef enum
{
    MENU,
    RACE,
    RACE_WITH_FRIEND,
    BEST_RESULTS,
    RESULTS,
    EXIT
} Menu_state;

int subscribe_all();

int unsubscribe_all();

#endif
