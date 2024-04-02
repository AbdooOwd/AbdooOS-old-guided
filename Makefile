# By @AbdooOwd

ASM?=nasm
CC?=gcc

ASM16?=i386-elf-as
CC16?=i386-elf-gcc
LD16?=i386-elf-ld

CFLAGS += -ffreestanding -m32 -g -c

BUILD_DIR?=bin
SRC_DIR?=src

OS_FILENAME?=OS.iso


C_SOURCES=$(wildcard $(SRC_DIR)/kernel/drivers/*.c $(SRC_DIR)/kernel/*.c $(SRC_DIR)/cpu/*.c)
C_OBJECTS=${C_SOURCES:.c=.o}

ASM_SOURCES=$(wildcard $(SRC_DIR)/boot/kernel/*.asm $(SRC_DIR)/cpu/*.asm)
ASM_OBJECTS=${ASM_SOURCES:.asm=.o}

H_SOURCES=$(wildcard $(SRC_DIR)/kernel/*.h $(SRC_DIR)/kernel/drivers/*.h $(SRC_DIR)/cpu/*.h)



.PHONY: all os-image always clean oops

all: always os-image

os-image: $(BUILD_DIR)/$(OS_FILENAME)

always:
	mkdir -p $(BUILD_DIR)

clean clear:
	rm -r $(BUILD_DIR)/*

# in case it puts the .o files in the source directory
oops:
	rm -r $(SRC_DIR)/*.o

# The image
$(BUILD_DIR)/$(OS_FILENAME): $(BUILD_DIR)/boot.bin $(BUILD_DIR)/full_kernel.bin $(BUILD_DIR)/zeroes.bin
	cat $^ > $@
# Add '$(BUILD_DIR)/zeroes.bin' to the left side to fill up the OS

# Assembly Booting
$(BUILD_DIR)/boot.bin: $(SRC_DIR)/boot/boot.asm
	$(ASM) $< -f bin -o $@

# compiles all the assembly code
%.o: %.asm $(ASM_OBJECTS)
	$(ASM) $< -f elf -o $@

# Concatenate all C files and header files into one mega_kernel.c
$(BUILD_DIR)/mega_kernel.c: $(H_SOURCES) $(C_SOURCES) 
	cat $^ > $@

# Compile mega_kernel.c to object file
$(BUILD_DIR)/mega_kernel.o: $(BUILD_DIR)/mega_kernel.c
	$(CC) $(CFLAGS) $< -o $@

# Sectors Filler
$(BUILD_DIR)/zeroes.bin: $(SRC_DIR)/boot/zeroes.asm
	$(ASM) $< -f bin -o $@

# Link boot code, kernel entry, and mega_kernel.o
$(BUILD_DIR)/full_kernel.bin: $(ASM_OBJECTS) $(BUILD_DIR)/mega_kernel.o
	$(LD16) -o $@ -Ttext 0x1000 $^ --oformat binary -Map $(BUILD_DIR)/linked.map
