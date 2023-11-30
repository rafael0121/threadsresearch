#ifndef SEM_H_
#define SEM_H_

#include <stdbool.h>

#define SEM_MAX 20
#define ACTIVE true 
#define INACTIVE false

// Semaphore ID
typedef unsigned sem_t;

// Verify if the sem_table was started
extern bool started;

//Semaphore struct
struct semaphore {
    int value;
    sem_t id;
    bool state;
    int lock;
};

// Semaphore table
extern struct semaphore sem_table[SEM_MAX];

// Inicializa semáforo
int init_sem (sem_t, int);

// Realiza uma operação de incremento.
void up_sem (sem_t);

// Realiza uma operação de decremento.
void down_sem (sem_t);

#endif
