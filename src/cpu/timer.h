#ifndef TIMER_H
#define TIMER_H

#include "../libc/types.h"

void init_timer(uint32_t freq);
int get_tick();

#endif
