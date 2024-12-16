#include "types.h"
#include "process_manager.h"

#define MAX_SEMAPHORES 10

typedef struct {
    int value;
    PCB *waiting_queue[MAX_PROCESSES];
    int head, tail;
} semaphore_t;

semaphore_t semaphores[MAX_SEMAPHORES];

// Inicializa un semáforo
void init_semaphore(int index, int initial_value) {
    semaphores[index].value = initial_value;
    semaphores[index].head = 0;
    semaphores[index].tail = 0;
}

// Operación wait
void wait(int index) {
    if (--semaphores[index].value < 0) {
        PCB *current_process = &processes[current_pid];
        current_process->state = PROCESS_BLOCKED;
        semaphores[index].waiting_queue[semaphores[index].tail] = current_process;
        semaphores[index].tail = (semaphores[index].tail + 1) % MAX_PROCESSES;
        switch_process();
    }
}

// Operación signal
void signal(int index) {
    if (++semaphores[index].value <= 0) {
        PCB *unblocked_process = semaphores[index].waiting_queue[semaphores[index].head];
        semaphores[index].head = (semaphores[index].head + 1) % MAX_PROCESSES;
        unblocked_process->state = PROCESS_READY;
    }
}
