#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

#define MAX 10
#define CONSUMER 10

unsigned count = 0;

struct{
    double *data;
    unsigned size;
    unsigned cur;
    sem_t* sem_addr;
}typedef buffer;

buffer * buffer_create(){

    buffer *b = malloc(sizeof(buffer));
   
    b->data = malloc(sizeof(double) * MAX);
    b->size = MAX;
    b->cur = 0;
    
    return b;
}

void * produce(){
    
    double *c = malloc(sizeof(double));
    unsigned n = rand() % 99999999;

    for(int i = 0; i<n; i++){
            double a = 34.123412;
            double b = 3.22412345;

            *c = a / b * i;
    }

    void *obj = c;
    
    return obj;
}

void * consume(void *obj){
    double *c = obj;
    
    unsigned n = rand() % 99999999;

    for(int i = 0; i<n; i++){
            double a = 34.123412;
            double b = 3.22412345;

            *c = a * b * i;
    }    
}

void push(void *obj, buffer *b){
    double *obj_p = obj;
    b->data[b->cur] = *obj_p;
    b->cur++;
}

void * pop(buffer *b){
    void *obj;
    
    obj = &b->data[b->cur-1];
    b->cur--;

    return obj;
}

void * writer(void *b){
    buffer *buf = b;

    while(true){

        if(buf->cur > 0){
            void *obj = pop(b);  
            printf("consumer\n");
            consume(obj);
        }else{
        }

    }
}

void * producer(void *b){
    buffer *buf = b;
    while(true){

        if(buf->cur < buf->size){
            void *obj = produce();
            printf("producer\n");
            push(obj, b);
        }else{
        }

    }
}

int main(){

    srand(time(NULL));

    //Alloc memory to the buffer.
    buffer *b = buffer_create();
    
    //Create semaphore
    b->sem_addr = sem_open("sem", O_CREAT);

    //Steps to create the threads-----.
    pthread_t consumer_id, producer_id;
    
    pthread_attr_t attr;
    
    pthread_attr_init(&attr);

    pthread_create (&producer_id, &attr, &producer, b);
    pthread_create (&consumer_id, &attr, &consumer, b);

    //---------------------------------
    
    //Join threads
    pthread_join (consumer_id, NULL);
    pthread_join (producer_id, NULL);

}



