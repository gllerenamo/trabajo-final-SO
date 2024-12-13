#include "lib/display.h"

void kernel_entry() {
    init();
    print("Hola mundo", 10, 0);
    while (1) {
        //Bucle para mantener el sistema funcionando
    }
}
