// #include "screen.h"
// #include "../core.h"

unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);
unsigned short port_word_in(unsigned short port);
void port_word_out(unsigned short port, unsigned short data);

int get_offset(int col, int row);
void set_cursor(int offset);
int get_cursor();

void print_char(char c, int col, int row);
void print_at(const char* message, int col, int row);
void print(const char* message);
void clear_screen();



void print_char(char c, int col, int row) {
    unsigned char* vid = (char*)0xb8000;
    int offset;

    if (col >= 0 && row >= 0) {
        offset = get_offset(col, row);
    }
    else {
        offset = get_cursor();
    }

    if (c == '\n') {
        int rows = offset / (2 * MAX_COLS);
        offset = get_offset(MAX_COLS - 1, rows);
    }
    else
        vid[offset] = c;

    set_cursor(offset + 2);
}

void print_at(const char* message, int col, int row) {
    if (col >= 0 && row >= 0) {
        set_cursor(get_offset(col, row));
    }

    int i = 0;
    while (message[i] != 0) {
        print_char(message[i++], col, row);
    }
}

void print(const char* message) {
    print_at(message, -1, -1);
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

void set_cursor(int offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, ((unsigned char)(offset >> 8)) & 0x00FF);
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, offset & 0x00FF);
}

int get_cursor() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);

    return offset * 2;
}

int get_offset(int col, int row) {
    return (row * MAX_COLS + col) * 2;
}