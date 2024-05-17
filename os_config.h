#ifndef OS_CONFIG_H
#define	OS_CONFIG_H

#define STACK_SIZE      32
#define MAX_TASKS       4
#define PIPE_SIZE       2
#define DYNAMIC_ALLOC   1       // (1) habilita ou (0) desabilita o mecanismo de alocação dinâmica

// Macros para a definição do escalonador
// 1 ativado
// 0 desativado
#define RR_SCHEDULER        1
#define PRIOR_SCHEDULER     0



#endif	/* OS_CONFIG_H */

