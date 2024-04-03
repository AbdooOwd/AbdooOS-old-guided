// // #include "./drivers/screen.h"
// // #include "util.h"

/* Reboots System by going back to the beginning of boot sector (WIP) */
extern void reboot_system();
int check_action(char* le_input);
void clear_screen();
void initialize_kernel();



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
    check_action(input);
    print(input);
    print("\n> ");
}

int check_action(char* le_input) {
    if (strcmp(le_input, "ping") == 0) {
        print("Pong!\n");
    }

    if (strcmp(le_input, "end") == 0) {
        print("Stopping the CPU. Bye!\n");
        asm volatile("hlt");

    }

    if (strcmp(le_input, "reboot") == 0) {
        print("Rebooting...\n");
        goto * 0x0;
    }

    if ((strcmp(le_input, "qwerty") == 0) || (strcmp(le_input, "azerty") == 0)) {
        change_layout(le_input);
        print("Switched layout.\n");
    }
}