//Definicion de tipos de datos

#ifndef TYPES_H
#define TYPES_H

//Colores
#define BLACK        0x00
#define BLUE         0x01
#define GREEN        0x02
#define CYAN         0x03
#define RED          0x04
#define MAGENTA      0x05
#define BROWN        0x06
#define LIGHT_GRAY   0x07
#define DARK_GRAY    0x08
#define LIGHT_BLUE   0x09
#define LIGHT_GREEN  0x0A
#define LIGHT_CYAN   0x0B
#define LIGHT_RED    0x0C
#define LIGHT_MAGENTA 0x0D
#define YELLOW       0x0E 
#define WHITE        0x0F 

#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define BUFFER_SIZE 80*25

#define STACK_SIZE 1024

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

//Structs
typedef struct {
    uint8_t text_c;
    uint8_t background_c;
} Color;

typedef struct {
	char *text;
	int length;
} string;

typedef struct {
    int posH;
    int posV;
} Cursor;

int len(const uint8_t *string);
int strcmp(string *str1, string *str2);

#endif