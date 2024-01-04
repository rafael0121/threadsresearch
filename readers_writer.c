#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>

#define MAX 10
#define READERS 2

struct buffer{
    double *data;
    unsigned size;
    unsigned tail;
    unsigned head;
    
    int readers;
    sem_t sem;
    pthread_mutex_t lock;
};

struct buffer * buffer_create ()
{

    struct buffer *b = malloc(sizeof(struct buffer));

    // init buffer variables
    b->data = malloc(sizeof(double) * MAX);
    b->size = MAX;
    b->readers = 0;

    //Init semaphore
    sem_init(&b->sem, 0, 1);

    
    return b;
}

void read_data (struct buffer *b)
{
    double c = b->data[rand() % b->size];

    for (int i = 0; i < 9200; i++) {
        double a = 34.123412;
        double b = 34.123412;
        c = a * b * i;
    }
}


void * reader(void *buf)
{
    struct buffer *b = buf;

    while(true){
        pthread_mutex_lock(&b->lock);

        b->readers++;
        if (b->readers == 1)
            sem_wait(&b->sem);

        pthread_mutex_unlock(&b->lock);

        read_data(b);

        pthread_mutex_lock(&b->lock);

        printf("R");

        b->readers--;
        if (b->readers == 0)
            sem_post(&b->sem);
        
        pthread_mutex_unlock(&b->lock);
    }
}

void write_data (struct buffer *b)
{
    double c = rand();

    for (int i = 0; i < 9200; i++) {

        double a = 34.123412;
        double b = 3.22412345;

        c = a / b * i;
    }

    b->data[rand() % b->size] = c;
}


void * writer (void *buf)
{
    struct buffer *b = buf;

    while (true) {
        sem_wait(&b->sem);
        write_data(b);
        printf("W");
        sem_post(&b->sem);
    }

}

int main(){

    srand(time(NULL));
   

    //Alloc memory to the buffer.
    struct buffer *b = buffer_create();

    //---------------------------------
    
    int ret_reader, ret_writer;
    ret_reader = 0;
    ret_writer = 0;

    //Steps to create the threads
    pthread_t writer_id, reader_id[READERS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create (&writer_id, &attr, &writer, b);

    for (int i = 0; i < READERS; i++)
        pthread_create (&reader_id[i], &attr, &reader, b);

    //---------------------------------
    
    //Join threads
    for (int i = 0; i < READERS; i++) {
        ret_reader = pthread_join (reader_id[i], NULL);
        
        if (ret_reader != 0) {
            printf("\nReader thread error\n");
            return -1;
        }
    }

    ret_writer = pthread_join(writer_id, NULL);
    
    if (ret_writer != 0) {
        printf("\nWriter thread error\n");
        return -1;
    }
    
    return 0;
}



