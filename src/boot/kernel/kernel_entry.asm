%define ENDL 0x0A

global _start

_start:
section .text
    [bits 32]
    [extern _kernel_main]
    
    call _kernel_main
    jmp $