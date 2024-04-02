
const char* old_logo[3] = {
    "  _                   __   __ \n",
    " |_| |_   _|  _   _  |  | |__ \n",
    " | | |_| |_| |_| |_| |__|  __|\n"
};

const char* new_logo[6] = {
    "   ___    _           _                        ______      ______\n",
    "  | _ |  | |         | |                      |      |    | _____|\n",
    " | |_| | | |__     __| |    ____     ____    |   __   |  | |____\n",
    " |  _  | |  _ |   | _  |   | __ |   | __ |   |  |__|  |   |____ |\n",
    " | | | | | |_| | | |_| |  | |__| | | |__| |  |        |   _____| |\n",
    " |_| |_| |____|   |____|   |____|   |____|    |______|   |______|\n"
};

void print_char(char c, int col, int row);
void print_at(char* message, int col, int row);
void print(char* message);
void clear_screen();
void reboot();

void main_() {

    for (int i = 0; i < 6; i++) {
        print((char*)new_logo[i]);
    }

    print("\n\n");

    print(" - Welcome to AbdooOS!\n");

    return;
}

// maybe it works?
void reboot() {
    print("\n ] Rebooting...\n");
    goto * 0x7c00;
}
