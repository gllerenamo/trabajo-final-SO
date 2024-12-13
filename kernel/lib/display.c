//Archivo para funciones de pantalla
#include "display.h"

char char_buffer[VGA_HEIGHT][VGA_WIDTH]; //El buffer de caracteres, su tamaño maximo es de 25x80

void clear(Color color) {
    for (int i = 0; i < VGA_HEIGHT*VGA_WIDTH; i++) {
        uint8_t* chptr = (uint8_t*) VGA_MEMORY;
        chptr[i*2 + 1] = (color.background_c << 4) | color.text_c;
        put_char(i, ' ');
    }
}

void save() {
    for (int x = 0; x < VGA_HEIGHT; x++) {
        for (int y = 0; y < VGA_WIDTH; y++) {
            uint8_t *chptr = (uint8_t *) VGA_MEMORY;
            char_buffer[x][y] = chptr[(x * 80 + y)*2 + 1];
        }
    }
}

void init() {
    Color color = {BLACK, BLUE};
    clear(color);
    save();
}
