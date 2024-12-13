//Archivo para funciones de pantalla
#include "display.h"

char char_buffer[25][80]; //El buffer de caracteres, su tamaño maximo es de 25x80

void clear(char color) {
    for (int i = 0; i < 25*80; i++) {
        unsigned char* chptr = (unsigned char*) 0xB8000;
        chptr[i*2 + 1] = color;
        put_char(i, ' ');
    }
}

void save() {
    for (int x = 0; x < 25; x++) {
        for (int y = 0; y < 80; y++) {
            char *chptr = (char *)0xb8000;
            char_buffer[x][y] = chptr[(x * 80 + y)*2 + 1];
        }
    }
}

void init() {
    clear(0x10); // 0x10 = Negro sobre azul
    save();
}
