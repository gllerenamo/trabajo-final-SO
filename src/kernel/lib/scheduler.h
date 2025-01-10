#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process_manager.h"

// Definir tiempo de quantum para Round-Robin (en milisegundos)
#define TIME_QUANTUM 1000  

void init_scheduler();
void schedule_next();
void run_scheduler();

#endif
