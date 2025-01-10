#include "lib/scheduler.h"
#include "lib/shell.h"

void launch_shell() {
    uint32_t registers[8] = {0};
    create_process(0, PROCESS_READY, shell, registers);
}

void kernel_entry() {
    //Inicializacion de componentes del sistema
    init_display();
    init_process_manager();
    init_interrupts();
    init_keyboard();
    
    string s = {"Bienvenido al sistema! Presione enter para comenzar ...", 54};
    print(s.text, s.length);
    shell();

    return;

    init_scheduler();
    run_scheduler();
}
