# AbdooOS

So, I made another OS also called "AbdooOS", but now I'll re-start it. Why?
Because [NanoByte](https://www.youtube.com/@nanobyte-dev) is a genius.
I'm serious! This guy is really smart! But I'm not... So I'll use the tutorial of that
[Daedalus Community Youtube Channel](https://www.youtube.com/@DaedalusCommunity).
It is simpler, and, uhhh... That's it. It's simpler.

Anyway, I'll be blogging about this project [here](https://abdooowd.hashnode.dev/)
_(and in this devlog)_ so I actually realize if I'm understanding what I'm doing
_(also to do that technic called **"Rubber Ducking"**, search about it)_.

## Table of stuff

-   [The Start](#the-start---31032024)
    -   [Boot Sector + Protected Mode](#boot-sector--protected-mode)
    -   [C code linked](#c-code-linked)
    -   [Printing chars but not strings](#printing-chars-but-not-strings)
    -   [C Linking Idea](#c-linking-idea)
-   [The Screen](#the-screen---01042024)
    -   [Basic Screen Drivers](#basic-screen-driver)

# The Start - [31/03/2024]

## Boot Sector + Protected Mode

So, now I have a working boot sector and kernel. The boot sector just loads some
disk stuff with the BIOS, then just turns off the BIOS, which means no more interrupts ); ,
and this to get into protected mode. And protected mode means 32-bits instead of 16-bits,
and this means: **more memory! YAY!!**

_"What does the boot sector do"_ you ask? Well it does plenty of things:

-   It sets up the stack to use C code later and also cuz it's useful.
-   Reads the disk for a kernel.
-   Sets up the GDT to enter protected mode.
-   Disable interrupts then enters protected mode to enter the kernel _(using `kernel_entry.asm`)_.
-   Of course, this boot sector has the boot signature `0xAA55` at the end of it after
    filling what's empty with zeroes.

## C code linked

Ok, I linked the boot sector with the kernel. Cool! Now I can code in C! Left me to code
basic drivers and some headers, cuz our current data types are kinda wacky _(too few)_.
But I'll probably do that later, I should also understand how to use the linker. And with that,
I should also update my `Makefile`.

> **Fun Fact!** : I actually coded the `Makefile` by myself (: !

## Printing chars but not strings

My kernel can print characters but no strings! When I use my `print_char` function, it prints
a beautiful char. But if I use my `print` function which prints a collection of `char`s, it displays
nothing! I gotta do some researches and tests...

Fixed! I did a little test and found out that if I do:

```c
char my_string[] = "This is a string!";
```

instead of:

```c
char* my_string = "This is a string!";
```

It works! So our problem is that we're

> _(strings in C are collection of chars)_.

## C Linking Idea

Alright, now I can link all of the C files together, but now the code doesn't work.
So I got an idea: it works if all the code is in one file, right? So we put all of
the C files' content into one big C file called `mega_kernel.c`. I'm smart (: (:

# The Screen - [01/04/2024]

## Basic Screen Drivers

Alright, printing chars is cute and all. But it'd be better to print string instantly
with a string. So I'll code the `print` function, how does it work? Easy! Pass it a string,
`message`, then pass it it's x and y positions, or `col` and `row`. But we don't wanna
always have to specify where we wanna print, so let's code our **cursor control!**.
_"What?"_ you say? Basically we'll just code a function that uses I/O ports to either
get the cursor's offset, or set its offset.

> By the way, I still have no idea what an I/O port is.

> **Fun _(but interesting)_ fact:** An offset is not only the positions of the characters,
> they are cells, if `offset % 2 == 0` it means that we're talking about an ASCII character
> cell, but if not then we're dealing with a color cell _(i.e grey on black by default)_.
