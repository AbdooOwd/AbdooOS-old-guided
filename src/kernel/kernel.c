// // #include "./drivers/screen.h"
// // #include "util.h"

/* Reboots System by going back to the beginning of boot sector (WIP) */
extern void reboot_system();
int check_action(char* le_input);
void clear_screen();

const char* small_logo[3] = {
    "  _                   __   __ \n",
    " |_| |_   _|  _   _  |  | |__ \n",
    " | | |_| |_| |_| |_| |__|  __|\n"
};

const char* thick_logo[6] = {
    "   ___    _           _                        ______      ______\n",
    "  | _ |  | |         | |                      |      |    | _____|\n",
    " | |_| | | |__     __| |    ____     ____    |   __   |  | |____\n",
    " |  _  | |  _ |   | _  |   | __ |   | __ |   |  |__|  |   |____ |\n",
    " | | | | | |_| | | |_| |  | |__| | | |__| |  |        |   _____| |\n",
    " |_| |_| |____|   |____|   |____|   |____|    |______|   |______|\n"
};

void main_() {
    clear_screen();

    for (int i = 0; i < sizeof(thick_logo) / 4; i++) {
        print((char*)thick_logo[i]);
    }

    print("\n\n");

    print(" - Welcome to AbdooOS!\n");

    isr_install();
    irq_install();

    asm volatile("sti");

    init_keyboard();
    print("> ");
}

// for handling commands (WIP)
char* already_action = "";

void user_input(char* input) {
    int can_action = check_action(input);
    if (can_action == 0) {
        print("Unknown Command: ");
    }
    print(input);
    print("\n> ");
}

int check_action(char* le_input) {
    if (strcmp(le_input, "END") == 0) {
        print("Stopping the CPU. Bye!\n");
        asm volatile("hlt");

        return 1;
    }

    if (strcmp(le_input, "REBOOT") == 0) {
        print("Rebooting...\n");
        goto * 0x0;

        return 1;
    }

    return 0;
}