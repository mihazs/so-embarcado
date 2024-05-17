#include "semaphore.h"
#include "kernel.h"
#include "scheduler.h"
#include <xc.h>

extern ready_queue_t Ready_queue;

// Rotinas para manipula��o de sem�foro (padr�o POSIX)
void sem_init(sem_t *s, uint_t valor)
{
    di();
    
    s->sem_cont                 = valor;
    s->sem_initial_value        = valor;
    s->sem_queue_size           = 0;
    s->sem_queue_pos_release    = 0;
    
    ei();
}

void sem_wait(sem_t *s)
{
    di();
    
    s->sem_cont--;
    if (s->sem_cont < 0) {
        // Tarefa ficar� bloqueada no sem�foro
        // at� que algu�m fa�a sem_post()
        s->sem_queue[s->sem_queue_size] = Ready_queue.task_running;
        s->sem_queue_size = (s->sem_queue_size + 1) % MAX_TASKS;
        // Salva contexto da tarefa que ficar� bloqueada
        SAVE_CONTEXT(WAITING_SEM);
        // Chama o escalonador para selecionar a pr�xima tarefa apta
        Ready_queue.task_running = scheduler();
        // Restaura o contexto da tarefa que foi selecionada para executar
        RESTORE_CONTEXT();        
    }
    
    ei();
}

void sem_post(sem_t *s)
{
    di();
    
    s->sem_cont++;
    
    if (s->sem_cont <= 0) {
        Ready_queue.tasks[s->sem_queue[s->sem_queue_pos_release]].task_STATE = READY;
        s->sem_queue_pos_release = (s->sem_queue_pos_release+1) % s->sem_queue_size;
        SAVE_CONTEXT(READY);
        // Chama o escalonador para selecionar a pr�xima tarefa apta
        Ready_queue.task_running = scheduler();
        // Restaura o contexto da tarefa que foi selecionada para executar
        RESTORE_CONTEXT();                
    }
    
    ei();    
}

