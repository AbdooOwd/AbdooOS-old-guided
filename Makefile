# By @AbdooOwd

ASM?=nasm
CC?=gcc

ASM16?=i386-elf-as
CC16?=i386-elf-gcc
LD16?=i386-elf-ld

CFLAGS += -ffreestanding -m32 -g -c

BUILD_DIR?=bin
SRC_DIR?=src

C_SOURCES=$(wildcard $(SRC_DIR)/kernel/*.c $(SRC_DIR)/kernel/drivers/*.c)
C_OBJECTS=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))

.PHONY: all os_image always clean

all: os_image

os_image: $(BUILD_DIR)/OS.bin

always:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/kernel
	mkdir -p $(BUILD_DIR)/kernel/drivers

clean:
	rm -r $(BUILD_DIR)/*

# The image
$(BUILD_DIR)/OS.bin: always $(BUILD_DIR)/boot.bin $(BUILD_DIR)/full_kernel.bin $(BUILD_DIR)/zeroes.bin
	cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/full_kernel.bin $(BUILD_DIR)/zeroes.bin  > $(BUILD_DIR)/OS.bin

# Assembly Booting
$(BUILD_DIR)/boot.bin: $(SRC_DIR)/boot/boot.asm
	$(ASM) $(SRC_DIR)/boot/boot.asm -f bin -o $(BUILD_DIR)/boot.bin

# transition from boot to kernel
$(BUILD_DIR)/kernel_entry.o: $(SRC_DIR)/boot/kernel_entry.asm
	$(ASM) $^ -f elf -o $@

# the kernel object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

# Sectors Filler
$(BUILD_DIR)/zeroes.bin: $(SRC_DIR)/boot/zeroes.asm
	$(ASM) $(SRC_DIR)/boot/zeroes.asm -f bin -o $(BUILD_DIR)/zeroes.bin

$(BUILD_DIR)/full_kernel.bin: $(BUILD_DIR)/kernel_entry.o $(C_OBJECTS)
	$(LD16) -o $(BUILD_DIR)/full_kernel.bin -Ttext 0x1000 $(BUILD_DIR)/kernel_entry.o $(C_OBJECTS) --oformat binary
