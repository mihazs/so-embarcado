/* main. */

#include "kernel.h"
#include "user_app.h"

/**
 * Fun��o principal do SO
 */
int main()
{
    config_hardware();
    init_zero();

    // Cria��o das tarefas de usu�rio
    create_task(1, 1, read_temperature);
    //create_task(1, 1, tarefa_2);
    
    start_os();
    
    while (1) {
        
    }
    
    return 0;
}
