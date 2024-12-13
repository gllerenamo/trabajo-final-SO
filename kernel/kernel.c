#include "lib/display.h"

void kernel_entry() {
    string s = {"Hola Mundo!", 11};
    init();
    print(s.text, s.length, 0);

    while (1) {
        //Bucle para mantener el sistema funcionando
    }
}
