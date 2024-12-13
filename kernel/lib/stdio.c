//Archivo para I/O consola
#include "stdio.h"

void put_char(int pos, unsigned char c) {
    if (pos < 0 || pos >= VGA_WIDTH * VGA_HEIGHT) return; // Posicion invalida
    char *video = (char *) VGA_MEMORY;
    video[pos*2] = c;
}

void print(const char *str, int lenght, int pos) {
    for (int i = 0; i < lenght; i++) {
        put_char(pos + i, *str);
        str++;
    }
}