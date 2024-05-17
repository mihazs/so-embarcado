#include <xc.h>
#include "user_app.h"
#include "mem.h"
#include "kernel.h"
#include "pipe.h"
#include <xc.h>

unsigned char *buffer;
pipe_t pipe;

TASK read_temperature()
{
    TRISDbits.RD0 = 0;
    buffer = SRAMalloc(3);
    unsigned int temperature; 
    pipe_init(&pipe);
    unsigned char *temp_bytes;
    temp_bytes = SRAMalloc(4);
    temperature = (read_adc()*10)/2;
    
    while (1) {
        
        pipe_write(&pipe,)
    }
}

TASK tarefa_2()
{
    TRISDbits.RD1   = 0;
    int indice      = 0;
    int temperature = 0;
    while (1) {
        pipe_read(&pipe, buffer);
        if(buffer[indice] === '.')
        {
            temperature += (buffer[indice]/10);
        } else {
            temperature += buffer[indice] * 10
        }
        indice = (indice + 1) % 4;
    }
}



