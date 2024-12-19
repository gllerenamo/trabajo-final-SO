#include "scheduler.h"
#include "timer.h"
#include "stdio.h"

extern uint32_t current_pid;         

static int ready_queue[MAX_PROCESSES]; 
static int queue_head = 0;             
static int queue_tail = 0;            
static int num_ready_processes = 0;    

void init_scheduler() {
    set_process_state(current_pid, PROCESS_RUNNING);
    set_program_counter(get_current_pc());

    asm volatile("mov %0, %%esp" : : "r"(get_process(current_pid).stack_pointer));

    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (get_process(i).state == PROCESS_READY) {
            ready_queue[queue_tail++] = get_process(i).pid; 
            num_ready_processes++;
        }
    }
}
//robin
void schedule_next() {
    if (num_ready_processes == 0) {
        cursor.posH = 0;
        cursor.posV = 0;
        print("No hay procesos listos.\n", 20);
        return;
    }

    int next_pid = ready_queue[queue_head];
    queue_head = (queue_head + 1) % MAX_PROCESSES;

    set_process_state(get_current_pc(), PROCESS_READY);
    set_process_state(next_pid, PROCESS_RUNNING);

    switch_process(next_pid);
    current_pid = next_pid;
}

void run_scheduler() {
    while (1) {
        delay_ms(TIME_QUANTUM); 
        schedule_next();
    }
}
