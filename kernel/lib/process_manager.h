#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "types.h"

#define MAX_PROCESSES 4 // Valor inicial

extern uint8_t stacks[MAX_PROCESSES][STACK_SIZE];

typedef enum {
    PROCESS_NEW,
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED,
    PROCESS_TERMINATED
} process_state_t;

// Process Control Block
typedef struct {
    uint32_t pid;
    process_state_t state;
    uint32_t program_counter;
    uint32_t *stack_pointer;
    uint32_t registers[8]; // Estados de registro (eax, ebx, ecx, etc)
} PCB;

void init_process_manager();
void create_process(int pid, process_state_t state, void (*entry_point)(), uint32_t *registers);
PCB get_process(int pid);
PCB get_current_process();
void set_process_state(int pid, process_state_t new_state);
void switch_process(int pid);

uint32_t* allocate_stack(int pid);

uint32_t get_current_pc();
void set_program_counter(uint32_t new_pc);

#endif