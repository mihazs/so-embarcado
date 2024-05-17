#ifndef KERNEL_H
#define	KERNEL_H

#include "types.h"

extern ready_queue_t Ready_queue;

void create_task(uint_t id, uint_t prior, t_ptr func);
void create_task_one_shot(t_ptr func);
void exit_task();
void init_zero();
void config_hardware();
void start_os();
void delay_task(uint_t time);
void idle();
void config_adc();
void config_timer0();
void config_int0();
void config_ports();
void __interrupt() isr_timer();

// Implementa��o do despachante
#define SAVE_CONTEXT(state) { \
    do { \
        uint_t stack_size   = 0; \
        if (Ready_queue.tasks[Ready_queue.task_running].task_STATE == RUNNING) { \
            Ready_queue.tasks[Ready_queue.task_running].task_WREG         = WREG; \
            Ready_queue.tasks[Ready_queue.task_running].task_BSRREG       = BSR; \
            Ready_queue.tasks[Ready_queue.task_running].task_STATUSREG    = STATUS; \
            while (STKPTR) { \
                Ready_queue.tasks[Ready_queue.task_running].task_STACK.stack_data[stack_size] = TOS; \
                stack_size++; \
                asm("POP"); \
            } \
            Ready_queue.tasks[Ready_queue.task_running].task_STACK.stack_size             = stack_size; \
            Ready_queue.tasks[Ready_queue.task_running].task_STATE                        = state; \
        } \
    } while (0); \
} \

#define RESTORE_CONTEXT() { \
    do { \
        uint_t stack_size   = Ready_queue.tasks[Ready_queue.task_running].task_STACK.stack_size; \
        WREG    = Ready_queue.tasks[Ready_queue.task_running].task_WREG; \
        BSR     = Ready_queue.tasks[Ready_queue.task_running].task_BSRREG; \
        STATUS  = Ready_queue.tasks[Ready_queue.task_running].task_STATUSREG; \
        STKPTR  = 0;\
        if (Ready_queue.tasks[Ready_queue.task_running].task_STATE == READY) { \
            if (stack_size > 0) { \
                do { \
                    asm("PUSH"); \
                    stack_size--; \
                    TOS = Ready_queue.tasks[Ready_queue.task_running].task_STACK.stack_data[stack_size]; \
                } while (stack_size); \
            } \
            else { \
                asm("PUSH"); \
                TOS = Ready_queue.tasks[Ready_queue.task_running].task_PTR; \
            } \
        } \
        Ready_queue.tasks[Ready_queue.task_running].task_STATE                        = RUNNING; \
    } while (0); \
} \


#endif	/* KERNEL_H */

