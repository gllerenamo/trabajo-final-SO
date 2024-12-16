#include "scheduler.h"
#include "timer.h"
#include "stdio.h"

extern PCB processes[MAX_PROCESSES]; 
extern uint32_t current_pid;         

static int ready_queue[MAX_PROCESSES]; 
static int queue_head = 0;             
static int queue_tail = 0;            
static int num_ready_processes = 0;    

void init_scheduler() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].state == PROCESS_READY) {
            ready_queue[queue_tail++] = processes[i].pid; 
            num_ready_processes++;
        }
    }
}
//robin
void schedule_next() {
    if (num_ready_processes == 0) {
        print("No hay procesos listos.\n", 20, 0);
        return;
    }

    int next_pid = ready_queue[queue_head];
    queue_head = (queue_head + 1) % MAX_PROCESSES;

    processes[current_pid].state = PROCESS_READY;

    processes[next_pid].state = PROCESS_RUNNING;

    switch_process(next_pid);
    current_pid = next_pid;
}

void run_scheduler() {
    while (1) {
        delay_ms(TIME_QUANTUM); 
        schedule_next();
    }
}
