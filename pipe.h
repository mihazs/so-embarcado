#ifndef PIPE_H
#define	PIPE_H

#include "os_config.h"
#include "types.h"
#include "semaphore.h"

typedef struct pipe {
    uint_t pos_read;
    uint_t pos_write;
    byte pipe_data[PIPE_SIZE];
    sem_t pipe_sem_write;
    sem_t pipe_sem_read;
} pipe_t;

// Rotinas para manipulação do pipe
void pipe_init(pipe_t *p);
void pipe_write(pipe_t *p, byte data);
void pipe_read(pipe_t *p, byte *data);



#endif	/* PIPE_H */

