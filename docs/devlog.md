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
-   [Interrupts](#interrupts---02042024)
    -   [Coding Interrupts](#coding-interrupts)
    -   [Functions Cross-over](#functions-cross-over)
    -   [Scrolling Fix](#scrolling)
    -   [Keyboard Input](#keyboard-input)

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

My kernel can print characters but no strings\*! When I use my `print_char` function, it prints
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

It works! So our problem is that in the boot sector, we're reading too few sectors,
and we have that file `zeroes.asm` which fills up about 54 sectors worth of 0s.
So instead of doing this in the boot sector _(`boot.asm`)_:

```asm
mov bx, KERNEL_LOCATION
mov dh, 2 ; in zeroes.asm we write about 54 sectors worth of bytes
```

We do this:

```asm
mov bx, KERNEL_LOCATION
mov dh, 54 ; in zeroes.asm we write about 54 sectors worth of bytes
```

And now it's fixed! Because assigning a variable puts it in the stack, but using
a value without it being a variable means it's in memory. This is why declaring a variable
worked and directly using a string didn't work.

> \* _(strings in C are collection of chars)_.

## C Linking Idea

Alright, now I can link all of the C files together, but now the code doesn't work.
So I got an idea: it works if all the code is in one file, right? So we put all of
the C files' content into one big C file called `mega_kernel.c`. I'm smart (: (:

# The Screen - [01/04/2024]

## Basic Screen Drivers

Alright, printing chars is cute and all. But it'd be better to print a string instantly.
So I'll code the `print` function, how does it work? Easy! Pass it a string,
`message`, then pass it it's x and y positions, or `col` and `row`. But we don't wanna
always have to specify where we wanna print, so let's code our **cursor control!**.
_"What?"_ you say? Basically we'll just code a function that uses I/O ports to either
get the cursor's offset\*, or set its offset.

> By the way, I still have no idea what an I/O port is.

But what if I wanna print a string at a specific line? Then I'll code the `print_at` function.
Oh wait! Can you smell that? I smell _optimizaiton_! Instead of repeating similar codes
in both functions `print` and `print_at`, we can make that if in `print_at` the column or row
is zero or negative then we print directly in the cursor's offset. And so `print` uses `print_at`
but with `-1` for the column and row.

> \* **Fun _(but interesting)_ fact:** An offset is not only the positions of the characters,
> they are cells, if `offset % 2 == 0` it means that we're talking about an ASCII character
> cell, but if not then we're dealing with a color cell _(i.e grey on black by default)_.

I added scrolling, I don't how this works. Just gonna `ctrl + c` `ctrl + v`.

# Interrupts - [02/04/2024]

## Coding interrupts

Until now, I don't know what I'm doing. Just copying \: .

Alright, about 3 hours into it... I still don't understand anything. But, hey!
At least now I know how to use functions between assembly and C!
How? Kind of easy actually! _(if you don't use arguments and return)_:

-   If the function is from assembly, add a `_` at the **start** of the function's name.
    And to use it from C, just write the function's name without that underscore.
-   If the function is from C, then add a `_` at the **end** of the function's name
    _(adding an underscore at the end is only for the main function)_.
    And to use it from Assembly, add an underscore at the start of the function's name.

Uhhhh, it's extremly weird. When testing those "interrupts" by doing:

```c
__asm__ __volatile__("int $2")
__asm__ __volatile__("int $3")
```

The OS just keep refreshing its screen. Weird really.

## Functions Cross-over

Alright! Interrupts still don't work... BUT NOW I KNOW HOW TO GIVE ARGUMENTS
TO FUNCTIONS THAT ARE IN C FORM ASSEMBLY!!!1!!
Let's take our `print` function as an example _(from `src/kernel/drivers/screen.c`)_.
It takes a `char*` as argument. So what we do, is do this
_(read the comments for clarification)_:

```asm
[extern _print]         ; to get the print function from our C file
mov ebx, our_message    ; put our message in a register so we push it to the stack later
push ebx                ; we push it to the stack so it is taken as argument

call _print             ; actually call the function with our arugment already given

; this is the message we'll print (0x0A is for a new line)
our_message: db "Hello from Assembly!", 0x0A, 0
```

This is actually kind of easy! Next step: how to get the return.

## Scrolling Fix

Alright, STILL NO INTERRUPTS... But now I fixed scrolling! So if I write too much text now,
it will normally scroll. I'm a genius _(haha no)_.

## Keyboard input

LEZGOOO!!!! I HAVE A WORKING SHELL THAT I (**ctrl+c** **ctrl+v**)'d.
Now I gotta code some basic commands and learn more C.
I suck at C. I need to learn C. I **MUST** learn **C**.

# The Shell - [03/04/2024]

## Basic Fixes

Alright, first I'll fix some stuff:

-   When pressing backspace, we don't delete. We print a weird character and don't go back.
    And on top of that, the cursor doesn't even move!
-   We can delete even previously printed text _(even text that was printed before enabling the keyboard)_.
    I can fix that by putting an if statement that stops the user from backspac-ing if our
    buffer _(i.e our input)_ is empty or null.

Alright! I fixed all those stuff. Our next task is to add lowercase uppercase letters.

## Lowercase and uppercase

So, I'm too dumb to know how to make the system realize the Left Shift key is held
and not only pressed once. So I'll make it a "toggle" thing. Seems easy enough.
