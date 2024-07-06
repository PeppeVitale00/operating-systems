//Scrivere un programma C che utilizza un mutex 
//per proteggere l'accesso a una variabile globale incrementata da più thread.

#define NUM_THREADS 4
#define INCREMENTS 1000

#include <stdio.h>
#include <pthread.h>

int shared_variable=0;
pthread_mutex_t mutex;

void* incremento (void* arg){
    pthread_mutex_lock(&mutex);
    shared_variable++;
    printf("Shared Variable increased by thread %ld. New value: %d\n",pthread_self(),shared_variable);
    pthread_mutex_unlock(&mutex);
}

int main(){
    
    pthread_mutex_init(&mutex, NULL);

    pthread_t thread[NUM_THREADS];

    for(int i=0;i<NUM_THREADS;i++){
        pthread_create(&thread[i],NULL, incremento, NULL);
    }

    for(int i=0;i<NUM_THREADS;i++){
        pthread_join(thread[i],NULL);
    }

    pthread_mutex_destroy(&mutex);

}