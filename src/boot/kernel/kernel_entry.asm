section .text
    [bits 32]

    [extern _main_]
    [extern _print]

    mov edx, idiot
    
    call _print
    call _main_
    jmp $

idiot: db "HELLO", 0