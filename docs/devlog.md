# AbdooOS

So, I made another OS also called "AbdooOS", but now I'll re-start it. Why?
Because [NanoByte](https://www.youtube.com/@nanobyte-dev) is a genius.
I'm serious! This guy is really smart! But I'm not... So I'll use the tutorial of that
[Daedalus Community Youtube Channel](https://www.youtube.com/@DaedalusCommunity).
It is simpler, and, uhhh... That's it. It's simpler.

Anyway, I'll be blogging about this project [here](https://abdooowd.hashnode.dev/)
_(and in this devlog)_ so I actually realize if I'm understanding what I'm doing
_(also to do that technic called **"Rubber Ducking"**, search about it)_.

# The Start - [31/03/2024]

So, now I have a working boot sector and kernel. The boot sector just loads some
disk stuff with the BIOS, then just turns off the BIOS, which means no more interrupts ); ,
and this to get into protected mode. And protected mode means 32-bits instead of 16-bits,
and this means: **more memory! YAY!!**

Ok, I linked the boot sector with the kernel. Cool! Now I can code in C! Left me to code
basic drivers and some headers, cuz our current data types are kinda wacky _(too few)_.
But I'll probably do that later, I should also understand how to use the linker. And with that,
I should also update my `Makefile`.

> **Fun Fact!** : I actually coded the `Makefile` by myself (: !

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
