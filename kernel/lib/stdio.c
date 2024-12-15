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
    chptr[pos*2 + 1] = 0xF0;
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
        if (pos % VGA_WIDTH == 0) {
            buffer[pos - 1] = 0;
        }
    }
}

Cursor key_handler(Cursor *position, uint8_t ch, uint8_t *buffer, int allow_backspace) {
    int H = position->posH;
    int V = position->posV;
    if (en_US[ch] == 0) return *position; //Posible mejora: manejar teclas como shift, ctrl, etc. 

    if (ch == 0x0E) { // Backspace
        if (H == 1) return *position;
        position->posH--;
        buffer[H + V * VGA_WIDTH] = 0;
        put_char(H + V * VGA_WIDTH, 0);
    } else if (ch == 0x1C) { // Enter
        position->posH = 0;
        position->posV++;
        // Si se supera el número de líneas (pantalla llena), desplazamos todo hacia arriba
        // Desplazar el contenido de la pantalla una línea arriba
        if (V >= VGA_HEIGHT) {
            move_up();
            position->posV = VGA_HEIGHT - 1;
        }
    } else {
        if (H >= VGA_WIDTH) {
            position->posH = 1;
            position->posV++;
            if (V >= VGA_HEIGHT) {
                move_up();
                position->posV = VGA_HEIGHT - 1;
            }
            return *position;
        }
        put_char(H + V * VGA_WIDTH, en_US[ch]);
        buffer[H + V * VGA_WIDTH] = ch;
        position->posH++;
    }
    buffer_handler(H + V * VGA_WIDTH, ch, buffer);
    return *position;
}

void move_up() {
    uint8_t *video = (uint8_t *) VGA_MEMORY;
    for (int i = 0; i < VGA_HEIGHT - 1; i++) {
        for (int j = 0; j < VGA_WIDTH; j++) {
            video[j + i * VGA_WIDTH] = video[j + (i + 1) * VGA_WIDTH];
        }
    }
    
    for (int i = 0; i < VGA_WIDTH; i++)
        video[i + (VGA_HEIGHT - 1) * VGA_WIDTH] = 0;
}

void input(Cursor *position, uint8_t *buffer, int allow_backspace) {
    uint8_t ch = 0;
    while (ch != 0x1C) {
        ch = get_char();
        *position = key_handler(position, ch, buffer, 1);
        draw_char(position->posH + position->posV * VGA_WIDTH);
        //debug(1 + position.posH, position.posV * VGA_WIDTH);
    }
}

//Funcion con propositos de debug
void put_int(int pos, int value) {
    char buffer[16];
    int i = 0;

    do {
        int rem = value % 10;
        buffer[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        value = value / 10;
    } while (value);

    buffer[i] = '\0';
    int start = 0;
    int end = i-1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }

    i = 0;
    while (buffer[i] != '\0') {
        uint8_t *video = (uint8_t *) VGA_MEMORY;
        video[(pos + i)*2] = buffer[i];
        i++;
    }
}
