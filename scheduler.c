#include "scheduler.h"
#include "os_config.h"
#include "kernel.h"

extern ready_queue_t Ready_queue;

uint_t priority_scheduler()
{
    // Considerando que a tarefa idle está na primeira posição
    // da fila de aptos (0)
    uint_t maior_prioridade = 0, i;
    
    for (i = 1; i < Ready_queue.queue_size; i++) {
        if (Ready_queue.tasks[i].task_STATE == READY) {
            maior_prioridade = i;
            break;
        }
    }
    
    if (maior_prioridade == 0) return 0;
    
    for (i = maior_prioridade; i < Ready_queue.queue_size-1; i++) {
        if (Ready_queue.tasks[i].task_PRIOR <
            Ready_queue.tasks[i+1].task_PRIOR &&
            Ready_queue.tasks[i+1].task_STATE == READY &&
            Ready_queue.tasks[i+1].task_PTR != idle) {
            maior_prioridade = i+1;
        }
    }
    
    return maior_prioridade;
}

uint_t round_robin_scheduler()
{
    uint_t next_task = Ready_queue.task_running, vez = 0;
    
    do {
        next_task = (next_task + 1) % Ready_queue.queue_size;
        if (next_task == 0) {
            vez++;
            if (vez == 2) return 0;
        }
        
    } while (Ready_queue.tasks[next_task].task_STATE != READY ||
             Ready_queue.tasks[next_task].task_PTR == idle);

    return next_task;
}

uint_t scheduler()
{
#if RR_SCHEDULER
    return round_robin_scheduler();
#elif PRIOR_SCHEDULER
    return priority_scheduler();
#endif    
}

