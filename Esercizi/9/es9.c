/*
Implementare un programma produttore-consumatore utilizzando mutex e variabili condizione.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


typedef struct{    
    int* buffer;
    int in;
    int out;
    int buffersize;
    int count;
}thread_data;

void* producer (void* arg){
    thread_data* data = (thread_data*) (arg); 
    for(int i=0;i<data->buffersize;i++){
        pthread_mutex_lock(&mutex);

        while(data->count==data->buffersize){
            pthread_cond_wait(&not_full,&mutex);
        }

        data->buffer[data->in]=i;
        data->in=(data->in+1) %data->buffersize;
        data->count++;

        printf("Prodotto: %d",i);

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);

}

void* consumer (void* arg){
    thread_data* data = (thread_data*) (arg);
    
    for(int i=0;i<data->buffersize;i++){
        pthread_mutex_lock(&mutex);

        while(data->count==0){
            pthread_cond_wait(&not_empty,&mutex);
        }

        int item = data->buffer[data->out];
        data->out=(data->out+1)%data->buffersize;
        printf("Consumato: %d",item);
        data->count--;

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);

}

int main(){

    int num_threads=2;
    thread_data data; 
    pthread_t* consumatore = malloc(sizeof(pthread_t)*2);
    pthread_t* produttore = malloc(sizeof(pthread_t)*2);

    data.buffer=malloc(sizeof(int)*10);
    data.in=0;
    data.out=0;
    data.count=0;
    data.buffersize=10;

    for(int i=0;i<num_threads;i++){
        pthread_create(&consumatore[i],NULL,consumer,(void*)&data);
        pthread_create(&produttore[i],NULL, producer,(void*)&data);
    }

    for(int i=0;i<num_threads;i++){
        pthread_join(produttore[i],NULL);
        pthread_join(consumatore[i],NULL);
    }




}