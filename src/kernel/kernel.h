#ifndef KERNEL_H
#define KERNEL_H

void user_input(char* input);

/* Reboots System by going back to the beginning of boot sector (WIP) */
extern void reboot_system();
int check_action(char* le_input);
void clear_screen();
void initialize_kernel();

#endif