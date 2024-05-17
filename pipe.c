#include "pipe.h"
#include "semaphore.h"

// Rotinas para manipulação do pipe
void pipe_init(pipe_t *p)
{
    p->pos_read     = 0;
    p->pos_write    = 0;
    sem_init(&p->pipe_sem_write, PIPE_SIZE);
    sem_init(&p->pipe_sem_read, 0);
}

void pipe_write(pipe_t *p, byte data)
{
    sem_wait(&p->pipe_sem_write);
    
    p->pipe_data[p->pos_write] = data;
    p->pos_write = (p->pos_write + 1) % PIPE_SIZE;
    
    sem_post(&p->pipe_sem_read);    
}

void pipe_read(pipe_t *p, byte *data)
{
    sem_wait(&p->pipe_sem_read);
    
    *data = p->pipe_data[p->pos_read];
    p->pos_read = (p->pos_read + 1) % PIPE_SIZE;
    
    sem_post(&p->pipe_sem_write);    
}

