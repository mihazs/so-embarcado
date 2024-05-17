#include "kernel.h"
#include <xc.h>
#include "scheduler.h"
#include "mem.h"

ready_queue_t Ready_queue;

void create_task(uint_t id, uint_t prior, t_ptr func)
{
    tcb_t new_task;
    
    new_task.task_ID                = id;
    new_task.task_PRIOR             = prior;
    new_task.task_PTR               = func;
    new_task.task_STACK.stack_size  = 0;
    new_task.task_STATE             = READY;
    
    Ready_queue.tasks[Ready_queue.queue_size]   = new_task;
    Ready_queue.queue_size                     += 1;    
}

void create_task_one_shot(t_ptr func)
{
    di();
    
    // Salva o contexto da tarefa que está atualmente em execução
    SAVE_CONTEXT(READY);
    
    // Instala a tarefa one shot na pilha
    STKPTR = 0;
    asm("PUSH");
    TOS = func;   
    
    ei();
}

void exit_task()
{
    di();
    
    Ready_queue.task_running = scheduler();
    RESTORE_CONTEXT();
    
    ei();   
}

void init_zero()
{
    Ready_queue.queue_size      = 0;
    Ready_queue.task_running    = 0;

    create_task(0, 0, idle);
    
#if DYNAMIC_ALLOC == 1
    SRAMInitHeap();
#endif    
}


void config_hardware()
{
    config_ports();
    config_adc();
    //config_timer0();
    //config_int0();
}

void start_os()
{
    //asm("GLOBAL _idle, _led_1, _led_2, _led_3, _one_shot_task");
    asm("GLOBAL _read_temperature, _tarefa_2");
    T0CONbits.TMR0ON    = 1;
}

void delay_task(uint_t time)
{
    // Desabilita as interrupções
    di();
    
    Ready_queue.tasks[Ready_queue.task_running].task_time_sleeping = time;
    
    // Salva contexto da tarefa que ficará aguardando por um tempo
    SAVE_CONTEXT(WAITING);
    // Chama o escalonador para selecionar a próxima tarefa apta
    Ready_queue.task_running = scheduler();
    // Restaura o contexto da tarefa que foi selecionada para executar
    RESTORE_CONTEXT();
    
    // Habilita as interrupções
    ei();
}

void idle() 
{
    while (1) {
        LATDbits.LATD7 = ~PORTDbits.RD7;
        Nop();
    }
}

void config_timer0()
{
    // Configurações de interrupção
    INTCONbits.PEIE     = 1;
    INTCONbits.TMR0IE   = 1;
    INTCONbits.TMR0IF   = 0;
    INTCONbits.GIE      = 1;
    // Configurações do TIMER0
    T0CONbits.T08BIT    = 1;
    T0CONbits.T0CS      = 0;
    T0CONbits.PSA       = 0;
    T0CONbits.T0PS      = 0b111;        // 1:32       
    TMR0                = 0;
}

void config_int0()
{
    INTCONbits.INT0IE   = 1;
    INTCONbits.INT0IF   = 0;
    INTCON2bits.INTEDG0 = 0;
    INTCON2bits.RBPU    = 0;    
}
void config_ports()
{
    TRISDbits.TRISD0 = 0;
    //TRISDbits.TRISD1 = 0;
    //TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD6 = 0;
    //TRISDbits.TRISD7 = 0;  
}

void __interrupt() isr_timer()
{
    if (INTCONbits.TMR0IF == 1) {
        // Verifica se tem tarefas em estado de WAITING
        for (int i = 0; i < Ready_queue.queue_size; i++) {
            if (Ready_queue.tasks[i].task_STATE == WAITING) {
                Ready_queue.tasks[i].task_time_sleeping--;
                if (Ready_queue.tasks[i].task_time_sleeping == 0) {
                    Ready_queue.tasks[i].task_STATE = READY;
                }
            }
        }

        SAVE_CONTEXT(READY);
        // Ready_queue.task_running = round_robin_scheduler();
        Ready_queue.task_running = scheduler();    
        RESTORE_CONTEXT();
        // Zera o flag do timer
        INTCONbits.TMR0IF   = 0;
        TMR0                = 0;
    }
    if (INTCONbits.INT0IF == 1) {
        // Cria a tarefa oneshot
        INTCONbits.INT0IF = 0;
        //create_task_one_shot(one_shot_task);
    }
}

