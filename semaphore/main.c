#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 10

unsigned count = 0;

struct{
    double *data;
    unsigned size;
    unsigned cur;
    pthread_mutex_t lock;
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

    for(int i = 0; i<93200000; i++){

        double a = 34.123412;
        double b = 3.22412345;

        *c = a / b * i;
    }

    void *obj = c;
    
    return obj;
}

void * consume(void *obj){
    double *c = obj;

    for(int i = 0; i<2; i++){
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

void * consumer(void *b){
    buffer *buf = b;

    while(true){
        pthread_mutex_lock(&buf->lock);

        if(buf->cur > 0){
            void *obj = pop(b);  
            pthread_mutex_unlock(&buf->lock);
            printf("consumer\n");
            consume(obj);
        }else{
            pthread_mutex_unlock(&buf->lock);
        }

    }
}

void * producer(void *b){
    buffer *buf = b;
    while(true){
        pthread_mutex_lock(&buf->lock);

        if(buf->cur < buf->size){
            void *obj = produce();
        printf("producer\n");
            pthread_mutex_unlock(&buf->lock);
            push(obj, b);
        }else{
            pthread_mutex_unlock(&buf->lock);
        }

    }
}

int main(){
    printf("Hello world!\n\n");
    
    pthread_t consumer_id, producer_id;

    void *b = buffer_create();
    
    pthread_attr_t attr;
    
    pthread_attr_init(&attr);

    pthread_create (&producer_id, &attr, &producer, b);
    pthread_create (&consumer_id, &attr, &consumer, b);

    pthread_join (consumer_id, NULL);
    pthread_join (producer_id, NULL);
}



