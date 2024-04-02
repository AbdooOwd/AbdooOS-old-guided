
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80

#define WHITE_ON_BLACK 0x0f

#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);
void set_cursor(int offset);
int get_cursor();
int handle_scrolling(int cursor_offset);

void print_char(char c, int col, int row);
void print_at(char* message, int col, int row);
void print(char* message);
void print_backspace();
void clear_screen();
