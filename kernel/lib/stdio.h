#include "types.h"

extern uint8_t shellbuffer[BUFFER_SIZE];

void init_buffer();
uint8_t get_char();
void put_char(int pos, uint8_t c);
void draw_char(int pos);
void fill(uint8_t color);
void print(const char *str, int lenght, int pos);
Cursor key_handler(Cursor *position, uint8_t ch, int allow_backspace);
void move_up();
void input(Cursor* position, int allow_backspace);
