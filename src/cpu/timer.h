#ifndef TIMER_H
#define TIMER_H

#include "../kernel/core/types.h"

void init_timer(u32 freq);
int get_tick();

#endif
