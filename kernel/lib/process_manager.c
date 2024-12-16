#include "process_manager.h"

uint32_t current_pid = 0;

void init_process_manager() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        processes[i].pid = 0;
        processes[i].state = PROCESS_NEW;
        processes[i].program_counter = 0;
        processes[i].stack_pointer = 0;
        for (int j = 0; j < 8; j++) {
            processes[i].registers[j] = 0;
        }
    }
}

void switch_process(int pid) {
    PCB *current_process = &processes[current_pid];
    asm volatile("mov %%esp, %0" : "=r"(current_process->stack_pointer)); //Guardar pila
    current_process->program_counter = 0;

    PCB *next_process = &processes[pid];
    set_program_counter(next_process->program_counter);
    asm volatile("mov %0, %%esp" : : "r"(next_process->stack_pointer)); //Cargar pila

    current_pid = pid;
}

uint32_t get_current_program_counter() {
    uint32_t pc;
    // En 32 bits, el registro EIP contiene la dirección de la siguiente instrucción a ejecutar
    // EIP: Extended Instruction Pointer
    __asm__ __volatile__(
        "call 1f\n"
        "1: pop %0\n"
        : "=r"(pc)
    );
    return pc;
}


void set_program_counter(uint32_t new_pc) {
    __asm__ __volatile__(
        "jmp *%0\n"  
        : // Sin salibidas
        : "r"(new_pc)
    );
}


