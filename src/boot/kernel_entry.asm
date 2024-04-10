%define ENDL 0x0A

global _start

_start:
section .text
    [bits 32]
    [extern kernel_main]
    [extern print]
    
    mov ebx, a
    push ebx
    call print
    pop ebx

    call kernel_main
    jmp $

a: db "Entering Kernel", 0