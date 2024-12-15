#include "lib/shell.h"

void kernel_entry() {
    string s = {"Bienvenido al sistema! Presione enter para comenzar ...", 54};
    init();
    print(s.text, s.length, 0);
    shell();
}
