%define ENDL 0x0A

section .text
    [bits 32]
    [extern _main_]
    
    call _main_
    jmp $