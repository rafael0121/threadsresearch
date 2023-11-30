#include <stdio.h>
#include "sem.h"

int main ()
{
    sem_t full = 0x2;
    sem_t empty = 0x3;
    
    if(init_sem(full, 0x10) == -1){
        printf("\n\nError init_sem(full)\n\n");
        return -1;       
    };

     if(init_sem(empty, 0x0) == -1){
        printf("\n\nError init_sem(empty)\n\n");
        return -1;       
    };


     up_sem(empty);


}

