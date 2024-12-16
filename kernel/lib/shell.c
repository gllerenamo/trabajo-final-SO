#include "shell.h"

string commands[1] = {
    {"clear", 5},
};

// Funciones auxiliares
void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void init_shell() {
    //Esperar a que se presione enter
    uint8_t ch;
    while (1) {
        ch = get_char();
        if (ch == 0x1C) break;
    }

    //Iniciar buffer con valores para espacio vacío
    init_buffer();

    //Limpiar pantalla
    Color c = {BLACK, BLUE};
    clear(c);
}

void process_command(Cursor *cursor) {
    int pos = 1 + (cursor->posV - 1) * VGA_WIDTH;
    char line_ch[VGA_WIDTH] = {0};
    string line = {line_ch, 0};

    while (1) {
        if (shellbuffer[pos] == ' ' || shellbuffer[pos] == 0x00 || pos % VGA_WIDTH == 0) break;
        line_ch[line.length] = shellbuffer[pos];
        pos++;
        line.length++;
    }

    if (strcmp(&line, &commands[0])) {
        Color c = {BLACK, BLUE};
        clear(c);
        cursor->posH = 0;
        cursor->posV = 0;
        print("clear ejecutado. Enter para continuar", 37, 0);
        init_shell();
    } else {
        print("Comando no encontrado", 21, cursor->posH + cursor->posV * VGA_WIDTH);
        cursor->posV++;
    }
}

void shell() {
    Cursor cursor = {0, 0};

    //Desactivar cursor
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);

    init_shell();

    while (1) {
        put_char(cursor.posH + cursor.posV * VGA_WIDTH, '$');
        shellbuffer[cursor.posH + cursor.posV * VGA_WIDTH] = '$';
        cursor.posH++;
        input(&cursor, 1);

        process_command(&cursor);

        if(cursor.posH + cursor.posV * VGA_WIDTH >= VGA_WIDTH * VGA_HEIGHT) {
            move_up();
            cursor.posV = VGA_HEIGHT - 1;
        }
    }
}
