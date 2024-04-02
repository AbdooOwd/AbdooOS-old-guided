# AbdooOS

An x86 OS I coded from scratch with the help of these resources:

-   [NanoByte](https://www.youtube.com/@nanobyte-dev).
-   [Daedalus Community Youtube Channel](https://www.youtube.com/@DaedalusCommunity).
-   Birmingham University OS-dev book by Nick Blundell.

# Prerequisites

-   A **Unix-like environment**:
    -   WSL or Cygwin for Windows _(I presonally used Cygwin)_
    -   Homebrew for MacOS
    -   If you use Linux, then you're done (:
-   Packages:
    -   `nasm` to compile your assembly code.
    -   `gcc` to compile your C code _(`gcc-g++` for Cygwin)_.
    -   `make` to build the project in one command.
    -   `i386-elf-ld` to link your code.
        -   This cannot be downloaded, you have to
            manually compile `binutils` with
            `i386-elf` as target.
            Refer to the [OS-Dev Wiki](https://wiki.osdev.org/).
    -   `qemu` to run your OS.

# How to run

Run `make` in the project's directory. You'll then find
a new directory `bin` which contains `OS.iso`
_(or the filename of the OS defined in `Makefile`)_.

You can now run that OS image using a Virutal Machine,
but I recommend using Qemu.

## How does it actually run?

-   First, it compiles the boot sector `src/boot/boot.asm`.
-   Then it compiles `full_kernel.bin` which needs
    more files:
    -   `kernel_entry.o` and this is compiled from
        the assembly file `src/boot/kernel/kernel_entry.asm`.
    -   `mega_kernel.o`: this is the merged version
        of all the C files and headers in the project.
        This is a **REALLY BAD** practice. I only did it
        because linking correctly wouldn't work for me.
        Please refer to some tutorials/resources like
        the [OS-Dev Wiki](https://wiki.osdev.org/) for more information.
-   Now he have the files:
    -   `boot.bin` _(our boot sector)_.
    -   `full_kernel.bin` _(our kernel entry + actual kernel)_.
    -   `zeroes.bin` _(a 512 bytes times 20 worth of 0s)_.
-   We concatenate these files into one
    beautiful file called **`OS.iso`**
    _(or whatever filename you defined for the OS' image)_.

## Available `make` commands

-   `os-image` : default command to build the OS' image.
-   `clean` or `clear` : delete the `bin`
    directory's content.
