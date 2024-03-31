#include "screen.h"

int get_offset(int col, int row);
void print_char(char c, int col, int row);
void print(const char* message);
void clear_screen();

void print_char(char c, int col, int row) {
    unsigned char* vid = (char*)0xb8000;
    int offset = get_offset(col, row);
    vid[offset] = c;
}

void print(const char* message) {
    int x = 0;
    int y = 0;

    int i = 0;

    while (message[i] != 0) {
        print_char(message[i++], x, y);

        x++; // Next char pos

        if (x >= MAX_COLS) {
            x = 0;
            y++;        // New line?
        }
    }
}

void clear_screen() {
    unsigned char* vidmem = (char*)0xb8000;
    unsigned int j = 0;
    while (j < MAX_COLS * MAX_ROWS * 2) {
        vidmem[j] = ' ';
        vidmem[j + 1] = 0x07;
        j = j + 2;
    }
}

int get_offset(int col, int row) {
    return (row * MAX_COLS + col) * 2;
}