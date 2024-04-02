// // #include "screen.h"
// // #include "../core.h"

// "low_level.c"
unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);
unsigned short port_word_in(unsigned short port);
void port_word_out(unsigned short port, unsigned short data);

int get_offset(int col, int row);
void set_cursor(int offset);
int get_cursor();
int handle_scrolling(int cursor_offset);

void print_char(char c, int col, int row);
void print_at(char* message, int col, int row);
void print(char* message);
void clear_screen();

void memory_copy(char* source, char* dest, int no_bytes);

void print_char(char c, int col, int row) {
    unsigned char* vid = (char*)VIDEO_ADDRESS;
    int offset;

    if (col >= 0 && row >= 0) {
        offset = get_offset(col, row);
    }
    else {
        offset = get_cursor();
    }

    if (c == '\n') {
        int rows = offset / (2 * MAX_COLS);
        offset = get_offset(MAX_COLS, rows);
    }
    else {
        vid[offset] = c;
        offset += 2;
    }

    //offset = handle_scrolling(offset);
    if (offset >= MAX_ROWS * MAX_COLS * 2) {
        int i;
        for (i = 1; i < MAX_ROWS; i++)
            memory_copy(get_offset(0, i) + vid,
                get_offset(0, i - 1) + vid,
                MAX_COLS * 2);
        char* last_line = get_offset(0, MAX_ROWS - 1) + vid;
        for (i = 0; i < MAX_COLS * 2; i++)
            if (i % 2 == 0) // so we don't hit the attribute byte (color)
                last_line[i] = 0;
        offset -= 2 * MAX_COLS;
    }

    set_cursor(offset);
}

/* FUNCITON DOESN'T WORK (prints last char of the string) */
void print_at(char* message, int col, int row) {
    if (!message)
        message = "No message provided to print!\n";

    if (col >= 0 && row >= 0) {
        set_cursor(get_offset(col, row));
    }

    int i = 0;
    while (message[i] != 0) {
        print_char(message[i++], col, row);
    }
}

void print(char* message) {
    print_at(message, -1, -1);
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

int handle_scrolling(int cursor_offset) {

    unsigned char* vidmem = (char*)VIDEO_ADDRESS;

    if (cursor_offset < MAX_ROWS * MAX_COLS * 2) {
        return cursor_offset;
    }

    int i;
    for (i = 1; i < MAX_ROWS; i++) {
        memory_copy(get_offset(0, i) + vidmem,
            get_offset(0, i - 1) + vidmem,
            MAX_COLS * 2
        );
    }

    /* Blank the last line by setting all bytes to 0 */
    char* last_line = get_offset(0, MAX_ROWS - 1) + vidmem;

    for (i = 0; i < MAX_COLS * 2; i++) {
        last_line[i] = 0;
    }
    cursor_offset -= 2 * MAX_COLS;

    return cursor_offset;
}

void clear_screen() {
    int row = 0;
    int col = 0;

    for (row = 0; row < MAX_ROWS; row++) {
        for (col = 0; col < MAX_COLS; col++) {
            print_char(' ', col, row);
        }
    }

    set_cursor(get_offset(0, 0));
}

int get_offset(int col, int row) {
    return (row * MAX_COLS + col) * 2;
}
