#ifndef SEMAPHORE_H
#define	SEMAPHORE_H

#include "os_config.h"
#include "types.h"

// Estrutura de dados de controle do semáforo
typedef struct sem {
    int sem_cont;
    uint_t sem_initial_value;
    uint_t sem_queue[MAX_TASKS];
    uint_t sem_queue_size;
    uint_t sem_queue_pos_release;
} sem_t;

// Rotinas para manipulação de semáforo (padrão POSIX)
void sem_init(sem_t *s, uint_t valor);
void sem_wait(sem_t *s);
void sem_post(sem_t *s);


#endif	/* SEMAPHORE_H */

