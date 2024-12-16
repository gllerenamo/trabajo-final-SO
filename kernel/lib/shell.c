#include "shell.h"

string commands[1] = {
    {"clear", 5},
};

// Funciones auxiliares
void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void process_command(uint8_t *buffer, Cursor *cursor) {
    int pos = 1 + (cursor->posV - 1) * VGA_WIDTH;
    char line_ch[VGA_WIDTH] = {0};
    string line = {line_ch, 1};

    while (1) {
        if (buffer[pos] == ' ' || buffer[pos] == 0x00 || pos % VGA_WIDTH == 0) break;
        line_ch[line.length] = buffer[pos];
        pos++;
        line.length++;
    }

    print(line.text, line.length, pos);
    print(line_ch, line.length, pos + VGA_WIDTH);
    print(commands[0].text, commands[0].length, pos + 2 * VGA_WIDTH);

    
    if (strcmp(&line, &commands[0])) {
        Color c = {BLACK, BLUE};
        clear(c);
    }
}

void shell() {
    uint8_t shellbuffer[BUFFER_SIZE];
    Cursor cursor = {0, 0};

    //Desactivar cursor
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);

    //Esperar a que se presione enter
    uint8_t ch;
    while (1) {
        ch = get_char();
        if (ch == 0x1C) break;
    }

    //Iniciar buffer con valores para espacio vacío
    for (int i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
        shellbuffer[i] = 0;
    }

    Color c = {BLACK, BLUE};
    clear(c);

    while (1) {
        put_char(cursor.posH + cursor.posV * VGA_WIDTH, '$');
        shellbuffer[cursor.posH + cursor.posV * VGA_WIDTH] = '$';
        cursor.posH++;
        input(&cursor, shellbuffer, 1);

        process_command(shellbuffer, &cursor);

        if(cursor.posH + cursor.posV * VGA_WIDTH >= VGA_WIDTH * VGA_HEIGHT) {
            move_up(shellbuffer);
            cursor.posV = VGA_HEIGHT - 1;
        }
    }
}
