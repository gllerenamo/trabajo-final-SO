CC=gcc
TARGET = kernel.ELF

MULTIBOOT_OBJ = multiboot.o

SRC_KERNEL = kernel.c
SRC_DIR = lib
SRC_LIB = $(wildcard $(SRC_DIR)/*.c)

OBJ_DIR = obj
OBJ_KERNEL = $(SRC_KERNEL:.c=.o)
OBJ_LIB = $(SRC_LIB:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJ_ALL = $(OBJ_KERNEL) $(OBJ_LIB)

CFLAGS = -ffreestanding -I$(SRC_DIR) -m32 -O2 -Wall

all: $(TARGET)

$(OBJ_KERNEL): $(SRC_KERNEL)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ_ALL) $(MULTIBOOT_OBJ)
	$(CC) -m32 -T linker.ld -o $(TARGET) -nostdlib -z noexecstack $(OBJ_ALL) $(MULTIBOOT_OBJ)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_ALL) $(TARGET)