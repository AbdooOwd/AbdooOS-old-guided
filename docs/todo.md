# AbdooOS' Todo List

## Basic Stuff

-   Make a working boot sector [X]
-   Make a working Kernel Entry to enter the kernel [X]
    -   Try make C work _(switch to higher-level programming language)_ [X]
    -   Code a C Kernel [X]
-   Link the C files [ FAILED ]
    -   Merge all the C files into `bin/mega_kernel.c` instead _(to then compile this file)_ [X]

## Drivers

-   Add "porting" _(I don't know what's that)_ [X]
-   Code a screen driver with basic printing function [X]
-   Code a keyboard driver to at least listen to letters' pressing [X]

## Libraries

-   Code a basic "util" library [X]
-   Code a `string` library [X]
-   Code a memory library [X]

## File System

-   Make a file system _(from command line with `mkfs.fat`)_ [X]
-   Load file system in boot sector [ ]
-   Code basic FAT12 tools _(in C)_ [ ]
    -   File reading [ ]
    -   File writing [ ]
    -   Directory Scanning _(kinda like `ls` but with more functions)_ [ ]

## Programs

-   _(temporary ToDo)_ Learn how to make _(external)_ programs [ ]
-   Load available programs [ ]
    -   Scan the available programs [ ]
    -   Be able to call/execute those programs with a command [ ]
-   Find an easy way to add programs to the OS without recompiling [ ]
    -   _<insert explanation on how to do it (if I did do this)>_

## Code Organizing / Refactoring

-   Make C code ideal for linking [X]
-   _(personal for computer)_ Recompile GCC for i386-elf target _"correctly"_ this time [X]
    -   Make `i386-elf-gcc` work [X]
-   Organize the code's paths [X]
