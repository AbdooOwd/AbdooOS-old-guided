# By @AbdooOwd

ASM?=nasm
CC?=gcc

ASM16?=i386-elf-as
CC16?=i386-elf-gcc
LD16?=i386-elf-ld

CFLAGS += -ffreestanding -m32 -g -c

BUILD_DIR?=bin
SRC_DIR?=src


C_SOURCES=$(wildcard $(SRC_DIR)/kernel/drivers/*.c $(SRC_DIR)/kernel/*.c)
C_OBJECTS=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))

ASM_SOURCES=$(wildcard $(SRC_DIR)/boot/kernel/*.asm)
ASM_OBJECTS=${ASM_SOURCES:.asm:.o}

H_SOURCES=$(wildcard $(SRC_DIR)/kernel/*.h $(SRC_DIR)/kernel/drivers/*.h)



.PHONY: all os-image always clean

all: os-image

os-image: $(BUILD_DIR)/OS.bin

always:
	mkdir -p $(BUILD_DIR)

clean clear:
	rm -r $(BUILD_DIR)/*


# The image
$(BUILD_DIR)/OS.bin: always $(BUILD_DIR)/boot.bin $(BUILD_DIR)/full_kernel.bin $(BUILD_DIR)/zeroes.bin
	cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/full_kernel.bin $(BUILD_DIR)/zeroes.bin > $(BUILD_DIR)/OS.bin
# Add '$(BUILD_DIR)/zeroes.bin' to the left side to fill up the OS

# Assembly Booting
$(BUILD_DIR)/boot.bin: $(SRC_DIR)/boot/boot.asm
	$(ASM) $(SRC_DIR)/boot/boot.asm -f bin -o $(BUILD_DIR)/boot.bin

# compiles all the assembly code
$(BUILD_DIR)/%.o: $(SRC_DIR)/boot/kernel/%.asm
	$(ASM) $^ -f elf -o $@

# Concatenate all C files and header files into one mega_kernel.c
$(BUILD_DIR)/mega_kernel.c: $(H_SOURCES) $(C_SOURCES) 
	cat $^ > $@

# Compile mega_kernel.c to object file
$(BUILD_DIR)/mega_kernel.o: $(BUILD_DIR)/mega_kernel.c
	$(CC) $(CFLAGS) $< -o $@

# Sectors Filler
$(BUILD_DIR)/zeroes.bin: $(SRC_DIR)/boot/zeroes.asm
	$(ASM) $(SRC_DIR)/boot/zeroes.asm -f bin -o $(BUILD_DIR)/zeroes.bin

# Link boot code, kernel entry, and mega_kernel.o
$(BUILD_DIR)/full_kernel.bin: $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/mega_kernel.o
	$(LD16) -o $(BUILD_DIR)/full_kernel.bin -Ttext 0x1000 $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/mega_kernel.o --oformat binary
