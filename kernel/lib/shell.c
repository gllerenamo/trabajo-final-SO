#include "shell.h"

string commands[1] = {
    {"clear", 5},
};

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

void keyboard_handler(uint8_t number) {
    print("Keyboard interrupt", 17);
    uint8_t ch = inb(0x60);
    keyboard_interrupt_handler(ch);
    outb(0x20, 0x20); // Enviar EOI al PIC
}

void process_command() {
    int pos = 1 + (cursor.posV - 1) * VGA_WIDTH;
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
        cursor.posH = 0;
        cursor.posV = 0;
        print("clear ejecutado. Enter para continuar", 37);
        init_shell();
    } else {
        print("Comando no encontrado", 21);
        cursor.posV++;
    }
}

void shell() {
    //Desactivar cursor
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);

    register_interrupt_handler(33, keyboard_handler);
    
    init_shell();

    while (1) {
        put_char(cursor.posH + cursor.posV * VGA_WIDTH, '$');
        shellbuffer[cursor.posH + cursor.posV * VGA_WIDTH] = '$';
        cursor.posH++;
        input(1);

        process_command(&cursor);

        if(cursor.posH + cursor.posV * VGA_WIDTH >= VGA_WIDTH * VGA_HEIGHT) {
            move_up();
            cursor.posV = VGA_HEIGHT - 1;
        }
    }
}
