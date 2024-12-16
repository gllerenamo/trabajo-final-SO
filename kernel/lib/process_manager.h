#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "types.h"

#define MAX_PROCESSES 4

typedef enum {
    PROCESS_NEW,
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED,
    PROCESS_TERMINATED
} process_state_t;

typedef struct {
    uint32_t pid;
    process_state_t state;
    uint32_t program_counter;
    uint32_t *stack_pointer;
    uint32_t registers[8];
} PCB;

extern PCB processes[MAX_PROCESSES];
extern uint32_t current_pid;

void init_process_manager();
void switch_process();

#endif