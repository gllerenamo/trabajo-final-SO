#include "process_manager.h"

uint32_t current_pid = 0;
PCB processes[MAX_PROCESSES];
uint8_t stacks[MAX_PROCESSES][STACK_SIZE];
uint32_t stack_poiter[MAX_PROCESSES];

void init_process_manager() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        processes[i].pid = 0;
        processes[i].state = PROCESS_NEW;
        processes[i].program_counter = 0;
        processes[i].stack_pointer = (uint32_t *)(stacks[i] + STACK_SIZE);
        for (int j = 0; j < 8; j++) {
            processes[i].registers[j] = 0;
        }
    }
}

void create_process(int pid, process_state_t state, void (*entry_point)(), uint32_t *registers) {
    PCB new_process;
    new_process.pid = pid;
    new_process.state = state;
    for (int i = 0; i < 0; i++) new_process.registers[i] = registers[i];

    uint32_t *stack_top = new_process.stack_pointer;
    *(--stack_top) = (uint32_t) entry_point; // Dirección de retorno
    new_process.stack_pointer = stack_top;
    new_process.program_counter = (uint32_t) entry_point;
}

PCB get_process(int pid) {
    return processes[pid];
}

PCB get_current_process() {
    return processes[current_pid];
}

void set_process_state(int pid, process_state_t new_state) {
    processes[pid].state = new_state;
}

void switch_process(int pid) {
    PCB *current_process = &processes[current_pid];
    asm volatile("mov %%esp, %0" : "=r"(current_process->stack_pointer)); //Guardar pila
    current_process->program_counter = get_current_pc();
    current_process->state = PROCESS_READY;

    PCB *next_process = &processes[pid];
    next_process->state = PROCESS_RUNNING;
    set_program_counter(next_process->program_counter);
    asm volatile("mov %0, %%esp" : : "r"(next_process->stack_pointer)); //Cargar pila
}

uint32_t* allocate_stack(int pid) {
    // * Manejar pid invalido *
    return (uint32_t *) &stacks[pid][STACK_SIZE - 1]; //Extremo superior de la pila
}

uint32_t get_current_pc() {
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
