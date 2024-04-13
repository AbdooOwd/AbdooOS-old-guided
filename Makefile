# By @AbdooOwd

ASM?=nasm
CC?=gcc

ASM16?=i386-elf-as
CC16?=/usr/local/i386elfgcc/bin/i386-elf-gcc
LD16?=i386-elf-ld

CFLAGS += -ffreestanding -c
ASMFLAGS +=

BUILD_DIR?=bin
SRC_DIR?=src

# Filenames
OS_FILENAME?=OS.iso
FAT_STD_FILENAME=fat_std.exe


C_SOURCES=$(wildcard $(SRC_DIR)/libc/*.c $(SRC_DIR)/cpu/*.c $(SRC_DIR)/drivers/*.c $(SRC_DIR)/kernel/*.c)
C_OBJECTS=${C_SOURCES:.c=.o}

ASM_SOURCES=$(wildcard $(SRC_DIR)/boot/kernel_entry.asm $(SRC_DIR)/cpu/*.asm $(SRC_DIR)/kernel/asm/*.asm)
ASM_OBJECTS=${ASM_SOURCES:.asm=.o}

OBJ=$(wildcard $(SRC_DIR)/*.o $(SRC_DIR)/boot/kernel/*.o $(SRC_DIR)/cpu/*.o $(SRC_DIR)/kernel/*.o $(SRC_DIR)/drivers/*.o $(SRC_DIR)/libc/*.o)

H_SOURCES=$(wildcard $(SRC_DIR)/kernel/core/*.h $(SRC_DIR)/libc/*.h $(SRC_DIR)/cpu/*.h $(SRC_DIR)/drivers/*.h $(SRC_DIR)/kernel/*.h)



.PHONY: all os-image always clean

all: os-image

os-image: always $(BUILD_DIR)/$(OS_FILENAME) $(BUILD_DIR)/fs_$(OS_FILENAME)
fat-std: $(BUILD_DIR)/$(FAT_STD_FILENAME)

always:
	mkdir -p $(BUILD_DIR)/objects
	mkdir -p $(BUILD_DIR)/info


clean-obj clear-obj:
	rm -r $(OBJ)

clean clear: clean-obj
	rm -r $(BUILD_DIR)/*

# The image
$(BUILD_DIR)/fs_$(OS_FILENAME): $(BUILD_DIR)/boot_nano.bin $(BUILD_DIR)/full_kernel.bin
	dd if=/dev/zero of=$@ bs=512 count=2880
	mkfs.fat -F 12 -n "NBOS" $@
	dd if=$(BUILD_DIR)/boot_nano.bin of=$@ conv=notrunc
	mcopy -i $@ $(BUILD_DIR)/full_kernel.bin "::kernel.bin"
	mcopy -i $@ src/data/poop.txt "::poop.txt"

# OG Image Rule
$(BUILD_DIR)/$(OS_FILENAME): $(BUILD_DIR)/boot.bin $(BUILD_DIR)/full_kernel.bin $(BUILD_DIR)/zeroes.bin
	cat $^ > $@

# Assembly Booting
$(BUILD_DIR)/boot.bin: $(SRC_DIR)/boot/boot.asm
	$(ASM) $< -f bin -o $@

$(BUILD_DIR)/boot_nano.bin: $(SRC_DIR)/boot/boot_nano.asm
	$(ASM) $< -f bin -o $@

# compiles all the assembly code
%.o: %.asm
	$(ASM) $< -f elf -o $@

%.o: %.c $(H_SOURCES)
	$(CC16) $(CFLAGS) $< -o $@

# Concatenate all C files and header files into one mega_kernel.c
#$(BUILD_DIR)/objects/mega_kernel.c: $(H_SOURCES) $(C_SOURCES) 
#	cat $^ > $@
#
## Compile mega_kernel.c to object file
#$(BUILD_DIR)/objects/mega_kernel.o: $(BUILD_DIR)/objects/mega_kernel.c
#	$(CC) $(CFLAGS) $< -o $@

# Sectors Filler
$(BUILD_DIR)/zeroes.bin: $(SRC_DIR)/boot/zeroes.asm
	$(ASM) $< -f bin -o $@

# Link boot code, kernel entry, and mega_kernel.o
$(BUILD_DIR)/full_kernel.bin: $(ASM_OBJECTS) $(C_OBJECTS)
	make always
	$(LD16) -o $@ -Ttext 0x1000 $^ --oformat binary -Map $(BUILD_DIR)/info/linked.map


# silly - test
$(BUILD_DIR)/$(FAT_STD_FILENAME): $(SRC_DIR)/tools/fat/fat.c 
	gcc $< -o $@

test-fat: fat-std $(BUILD_DIR)/fs_$(OS_FILENAME)
	./bin/$(FAT_STD_FILENAME) bin/fs_OS.iso "POOP    TXT"
