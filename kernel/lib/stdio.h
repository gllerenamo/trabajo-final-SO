#include "types.h"

extern uint8_t shellbuffer[BUFFER_SIZE];
extern Cursor cursor;

void init_buffer();
uint8_t get_char();
void put_int(int pos, int value);
void put_char(int pos, uint8_t c);
void draw_char(int pos);
void fill(uint8_t color);
void print(const char *str, int lenght);
void print_next_line(const char *str, int lenght);
void key_handler(uint8_t ch, int allow_backspace);
void move_up();
void input(int allow_backspace);
