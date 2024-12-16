#include "scheduler.h"

#define MAX_SEMAPHORES 10

extern PCB processes[MAX_PROCESSES];
extern uint32_t current_pid;

typedef struct {
    int value;
    PCB *waiting_queue[MAX_PROCESSES];
    int head, tail;
} semaphore_t;

semaphore_t semaphores[MAX_SEMAPHORES];

void init_semaphore(int index, int initial_value);
void wait(int index);
void signal(int index);