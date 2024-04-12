#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../libc/util.h"
#include "../libc/string.h"
#include "../cpu/isr.h"

#include "../libc/mem.h"

#include "kernel.h"


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

const char* help_message[] = {
    "Usage: help",
    "Available commands:\n",
    "ping: just prints \"Pong!\" back",
    "end: stops the CPU",
    "reboot: restarts the kernel",
    "qwerty | azerty: changes the keyboard layout to one of them",
    "theme: changes theme to \"dark\" or \"light\" based on current one"
};

char* previous_command = '\0';


void kernel_main() {
    initialize_kernel();

    for (int i = 0; i < sizeof(thick_logo) / 4; i++) {
        print((char*)thick_logo[i]);
    }

    print("\n\n - Welcome to AbdooOS!\n> ");
}

/* just some stuff to set up in the kernel at start */
void initialize_kernel() {
    clear_screen();
    clear_buffer(); // clear buffer if it's still in memory
    isr_install();
    irq_install(); // setting up interrupts
    asm volatile("sti");
    init_keyboard();
}


void user_input(char* input) {
    if (check_action(input) == 0)
        print(input);
    print("\n> ");
}

void execute_command(char* le_command) {
    if (strcmp(le_command, "ping") == 0) {
        print("Pong!");
    }
}

int check_action(char* le_input) {

    lower(le_input);

    if (!has_char(le_input, ' '))
        previous_command = le_input;

    if (strcmp(le_input, "ping") == 0) {
        print("Pong!");
        return 1;
    }
    else if (strcmp(le_input, "help") == 0) {
        for (int i = 0; i < sizeof(help_message) / 4; i++) {
            print((char*)help_message[i]);
            print("\n");
        }
        return 1;
    }
    else if (strcmp(le_input, "end") == 0) {
        print("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
        return 1;

    }
    else if (strcmp(le_input, "reboot") == 0) {
        print("Rebooting...\n");
        goto * 0x1000;
        return 1;
    }
    else if ((strcmp(le_input, "qwerty") == 0) || (strcmp(le_input, "azerty") == 0)) {
        change_layout(le_input);
        print("Switched layout.");
        return 1;
    }
    else if (strcmp(le_input, "theme") == 0) {
        change_theme();
        print("Changed Theme.");
        return 1;
    }
    else if (strcmp(le_input, "test") == 0) {
        char* text = "we are testing the split function";
        char* poop[] = {};
        int nono = split(text, ' ', poop);

        //print(poop[0]);

        return 1;
    }
    return 0;
}