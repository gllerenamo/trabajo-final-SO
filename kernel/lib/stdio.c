//Archivo para I/O consola
#include "stdio.h"
#include "keyboard.h"

uint8_t get_char() {
    uint8_t ch;

    while (1) {
        uint8_t status;
        asm volatile("inb $0x64, %0" : "=a"(status));
        if (status & 0x1) { // Bit de estado 0 indica que hay un byte en el buffer
            asm volatile("inb $0x60, %0" : "=a"(ch));
            if (ch & 0x80) continue; // Tecla liberada, no se devuelve nada
            return ch; // Tecla presionada, se devuelve valor de la tecla
        }
    }
}

void put_char(int pos, uint8_t c) {
    if (pos < 0 || pos >= VGA_WIDTH * VGA_HEIGHT) return; // Posicion invalida
    uint8_t *video = (uint8_t *) VGA_MEMORY;
    video[pos*2] = c;
}

void draw_char(int pos) {
    uint8_t *chptr = (uint8_t *) VGA_MEMORY;
    fill(0x0F);
    chptr[pos*2 + 1] = 0x21;
}

void fill(uint8_t color) {
    for (int i = 0; i < VGA_HEIGHT*VGA_WIDTH; i++) {
        uint8_t* chptr = (uint8_t*) VGA_MEMORY;
        chptr[i*2 + 1] = color;
    }
}

void print(const char *str, int lenght, int pos) {
    for (int i = 0; i < lenght; i++) {
        put_char(pos + i, str[i]);
    }
}

void buffer_handler(uint32_t pos, uint8_t ch, uint8_t *buffer) {
    if (pos == BUFFER_SIZE) {
        for (int i = 0; i < BUFFER_SIZE - 1; i++) {
            buffer[i] = buffer[i + 1];
        }
        buffer[BUFFER_SIZE - 1] = ch;
    } else if (ch == 0x0E) {
        buffer[pos - 1] = 0;
    } else {
        buffer[pos] = ch;
    }
}

Cursor key_handler(Cursor position, uint8_t ch, int min, uint8_t *buffer, int allow_backspace) {
    if (position.posH + position.posV * VGA_WIDTH >= VGA_WIDTH * VGA_HEIGHT && ch != 0x0E) { // Limite de pantalla
        position.posH = 0;
        position.posV = 0;
        return position;
    }
    if (ch == 0x0E && position.posH > min) { // Backspace
        position.posH--;
        if (allow_backspace) {
            buffer[position.posH] = 0;
            put_char(position.posH, ' ');
        }
    } else if (ch == 0x1C) { // Enter
        position.posH = 0;
        position.posV++;
    } else {
        put_char(position.posH, en_US[ch]);
        buffer[position.posH] = ch;
        position.posH++;
    }
    buffer_handler(position.posH + position.posV * VGA_WIDTH, ch, buffer);
    return position;
}

void input(int length, uint8_t *buffer, int line, int allow_backspace) {
    Cursor position = {length, line};
    int index = 0;
    uint8_t ch = 0;
    while (ch != 0x1C) {
        ch = get_char();
        position = key_handler(position, ch, 0, buffer, 1);
        draw_char(position.posH + position.posV * VGA_WIDTH);
        if (index == BUFFER_SIZE) index--;
        else index++;
    }

}
