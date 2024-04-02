# AbdooOS

An x86 OS I coded from scratch with the help these resources:

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
    -   `gcc` to compile your code _(`gcc-g++` for Cygwin)_.
    -   `make` to build the project in one command.
    -   `i386-elf-ld` to link your code.
        -   This cannot be downloaded, you have to
            manually compile `binutils` with
            `i386-elf` as target.
            Refer to the [OS-Dev Wiki](#).
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
        the [OS-Dev Wiki](#) for more information.
