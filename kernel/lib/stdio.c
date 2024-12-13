//Archivo para I/O consola
#include "stdio.h"

void put_char(int pos, uint8_t c) {
    if (pos < 0 || pos >= VGA_WIDTH * VGA_HEIGHT) return; // Posicion invalida
    uint8_t *video = (uint8_t *) VGA_MEMORY;
    video[pos*2] = c;
}

void print(const uint8_t *str, int lenght, int pos) {
    for (int i = 0; i < lenght; i++) {
        put_char(pos + i, *str);
        str++;
    }
}