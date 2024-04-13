#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../libc/types.h"

void init_keyboard();
void clear_buffer();
void change_layout(char* layout);

#endif