#include "keyboard.h"
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static int read_idx = 0;
static int write_idx = 0;
static int buffer_count = 0;

unsigned char en_US[128]=
{
	0,0,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
	'\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
	0, /* Ctrl (It's used to activate the function keys)*/
	'a','s','d','f','g','h','j','k','l',';', '\'','`',0, /* Left Shift */
	'\\','z','x','c','v','b','n','m',',','.','/', 0,/* Right shift */
	'*', 0, /* Alt */
	' ',
	0, /* Caps lock*/
	'H',0,0,0,0,0,0,0,0,0, /* F1-F10 keys */
	0, /* Num lock */
	0, /* Scroll lock */
	0, /* Home key */
	0, /* Up arrow */
	0, /* Page up */
	'-',
	0, /* Left arrow */
	0,
	0, /* Right arrow */
	'+',
	0, /* End key */
	0, /* Down arrow */
	0, /* Page down */
	0, /* Insert key */
	0, /* Delete key */
	0,0,0,
	'R', 0, /* F11-F12 Keys */
	0
};


void keyboard_interrupt_handler(uint8_t scancode) {
	if (scancode & 0x80) {
		// Tecla liberada
		return;
	}

	if (buffer_count < KEYBOARD_BUFFER_SIZE) {
		keyboard_buffer[write_idx] = en_US[scancode];
		write_idx = (write_idx + 1) % KEYBOARD_BUFFER_SIZE;
		buffer_count++;
	}
}

uint8_t get_char() {
    while (buffer_count == 0) {
        asm volatile("hlt");
    }
    uint8_t ch = keyboard_buffer[read_idx];
    read_idx = (read_idx + 1) % KEYBOARD_BUFFER_SIZE;
    buffer_count--;

    return ch;
}

void init_keyboard() {
	register_interrupt_handler(33, keyboard_handler);
}
