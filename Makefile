# By @AbdooOwd

ASM?=nasm
CC?=gcc

ASM16?=i386-elf-as
CC16?=i386-elf-gcc
LD16?=i386-elf-ld

CFLAGS += -ffreestanding -m32 -g -c -Werror

BUILD_DIR?=bin
SRC_DIR?=src

OS_FILENAME?=OS.iso


C_SOURCES=$(wildcard $(SRC_DIR)/kernel/drivers/*.c $(SRC_DIR)/kernel/*.c $(SRC_DIR)/cpu/*.c $(SRC_DIR)/libc/*.c)
C_OBJECTS=${C_SOURCES:.c=.o}

ASM_SOURCES=$(wildcard $(SRC_DIR)/boot/kernel/*.asm $(SRC_DIR)/cpu/*.asm)
ASM_OBJECTS=${ASM_SOURCES:.asm=.o}

OBJ=$(wildcard $(SRC_DIR)/*.o $(SRC_DIR)/boot/kernel/*.o $(SRC_DIR)/cpu/*.o $(SRC_DIR)/kernel/*.o $(SRC_DIR)/kernel/drivers/*.o)

H_SOURCES=$(wildcard $(SRC_DIR)/kernel/core/*.h $(SRC_DIR)/kernel/*.h $(SRC_DIR)/kernel/drivers/*.h $(SRC_DIR)/cpu/*.h $(SRC_DIR)/libc/*.h)



.PHONY: all os-image always clean

all: os-image

os-image: $(BUILD_DIR)/$(OS_FILENAME)

always:
#	mkdir -p $(BUILD_DIR)/asm


clean-obj clear-obj:
	rm -r ${OBJ}

clean clear: clean-obj
	rm -r $(BUILD_DIR)/*

# The image
$(BUILD_DIR)/$(OS_FILENAME): $(BUILD_DIR)/boot.bin $(BUILD_DIR)/full_kernel.bin
	dd if=/dev/zero of=$(BUILD_DIR)/pre_OS.iso bs=512 count=2880
	mkfs.fat -F 12 -n "NBOS" $(BUILD_DIR)/pre_OS.iso
	dd if=$(BUILD_DIR)/boot.bin of=$(BUILD_DIR)/pre_OS.iso conv=notrunc
#	mcopy -i $(BUILD_DIR)/pre_OS.iso $(BUILD_DIR)/full_kernel.bin "::full_kernel.bin"
#	mcopy -i $(BUILD_DIR)/pre_OS.iso $(BUILD_DIR)/zeroes.bin "::zeroes.bin"
	cat $^ $(BUILD_DIR)/pre_OS.iso > $@


#$(BUILD_DIR)/$(OS_FILENAME): $(BUILD_DIR)/boot.bin $(BUILD_DIR)/full_kernel.bin $(BUILD_DIR)/zeroes.bin
#	cat $^ > $@

# Assembly Booting
$(BUILD_DIR)/boot.bin: $(SRC_DIR)/boot/boot.asm
	$(ASM) $< -f bin -o $@

# compiles all the assembly code
%.o: %.asm
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
