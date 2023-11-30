#include "sem.h"

// Init semaphore table
struct semaphore sem_table[SEM_MAX];

// Init variable started
bool started = false;

// Initialize semaphore table values.
void start_sem()
{
    if (started == true)
        return;
    
    started = true;

    for (int i = 0; i < SEM_MAX; i++) {
        sem_table[i].state = INACTIVE;
    } 
    

    return;
}

// Init semaphore with the id received.  
int init_sem (sem_t id, int value)
{
    start_sem();

    for (int i =0; i<SEM_MAX; i++) {
        if (sem_table[i].state == INACTIVE) {

            sem_table[i].state = ACTIVE;
            sem_table[i].id = id;
            
            asm ("mov %1, %%ebx;"
                 "XCHG %%ebx, %0;"
                 : "=r" (sem_table[i].value)
                 : "r" (value)
                 : "%ebx");

            if (sem_table[i].value == value)
                return 1;
            else
                return -1;
        }
    }
    return -1;
};

void up_sem (sem_t id)
{
    struct semaphore *sem = 0x0;

    for (int i =0; i<SEM_MAX; i++) {
        if (sem_table[i].state == ACTIVE && sem_table[i].id == id) {
            sem = &sem_table[i];
            break;
        }
    }  
    
    if (sem == 0x0)
        return;
    
    asm ("movl $0, %%ebx;"
         "movl $1, %%eax;"
         "XCHG %%ebx, %0;"
         "addl %%eax, %%ebx;" 
         "XCHG %%ebx, %0;"
         : "=c" (sem->value)
        );


}

void down_sem (sem_t id)
{
     

}
