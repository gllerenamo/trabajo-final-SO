// concurrency_scheduler.c
#include "process_manager.h"
#include "timer.h"
#include "display.h"

#define TIME_SLICE 50 // Milisegundos por proceso

extern uint32_t current_pid;

// Estructura de semáforo
typedef struct {
    int value;
    PCB *waiting_queue[MAX_PROCESSES];
    int front;
    int rear;
} semaphore_t;

// Inicializar semáforo
void semaphore_init(semaphore_t *sem, int initial_value) {
    sem->value = initial_value;
    sem->front = 0;
    sem->rear = 0;
}

// Operación wait
void semaphore_wait(semaphore_t *sem) {
    disable_interrupts();
    sem->value--;
    if (sem->value < 0) {
        sem->waiting_queue[sem->rear] = &processes[current_pid];
        sem->rear = (sem->rear + 1) % MAX_PROCESSES;
        processes[current_pid].state = PROCESS_BLOCKED;
        scheduler();
    }
    enable_interrupts();
}

// Operación signal
void semaphore_signal(semaphore_t *sem) {
    disable_interrupts();
    sem->value++;
    if (sem->value <= 0) {
        PCB *unblocked_process = sem->waiting_queue[sem->front];
        sem->front = (sem->front + 1) % MAX_PROCESSES;
        unblocked_process->state = PROCESS_READY;
    }
    enable_interrupts();
}

// Planificador simple Round-Robin
void scheduler() {
    uint32_t next_pid = (current_pid + 1) % MAX_PROCESSES;

    // Buscar el próximo proceso listo para ejecutarse
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[next_pid].state == PROCESS_READY) {
            switch_process(next_pid);
            return;
        }
        next_pid = (next_pid + 1) % MAX_PROCESSES;
    }
}

// Interrupción de temporizador
void timer_interrupt_handler() {
    scheduler();
}

void init_concurrency() {
    // Configurar el temporizador
    init_timer(TIME_SLICE, timer_interrupt_handler);

    // Inicializar procesos
    init_process_manager();

    // Crear procesos de ejemplo (simulación)
    create_process(process_1);
    create_process(process_2);
    create_process(process_3);

    // Cambiar al primer proceso
    current_pid = 0;
    processes[current_pid].state = PROCESS_RUNNING;
    switch_process(current_pid);
}

// Semáforo compartido entre procesos
semaphore_t shared_resource_semaphore;

// Procesos simulados
void process_1() {
    while (1) {
        semaphore_wait(&shared_resource_semaphore);
        print("Proceso 1 accediendo al recurso compartido\n");
        for (volatile int i = 0; i < 1000000; i++); // Simular trabajo
        semaphore_signal(&shared_resource_semaphore);
    }
}

void process_2() {
    while (1) {
        semaphore_wait(&shared_resource_semaphore);
        print("Proceso 2 accediendo al recurso compartido\n");
        for (volatile int i = 0; i < 1000000; i++);
        semaphore_signal(&shared_resource_semaphore);
    }
}

void process_3() {
    while (1) {
        semaphore_wait(&shared_resource_semaphore);
        print("Proceso 3 accediendo al recurso compartido\n");
        for (volatile int i = 0; i < 1000000; i++);
        semaphore_signal(&shared_resource_semaphore);
    }
}