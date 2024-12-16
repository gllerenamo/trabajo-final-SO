#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "types.h"

#define MAX_PROCESSES 4 // Valor inicial

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
void switch_process(int pid);

uint32_t get_current_pc();
void set_program_counter(uint32_t new_pc);

#endif