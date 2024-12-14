#include "lib/display.h"

void kernel_entry() {
    string s = {"Hola Mundo!", 11};
    init();
    print(s.text, s.length, 0);
    unsigned char shellbuffer[BUFFER_SIZE];
    put_char(80, '$');
    while (1) {
        input(2, shellbuffer, 80, 1);
    }
}
