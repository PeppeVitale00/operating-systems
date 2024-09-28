#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 10


typedef struct{
    int buffer[BUFFER_SIZE];
    int buffer_head;
    int buffer_tail;
    pthread_mutex_t mutex;
    sem_t empty;
    sem_t full;

}shared_data_t;

typedef struct{
    pthread_t thread_id;
    shared_data_t* shared_data;
    int index;
    int iteration;

} producer_args_t;


typedef struct{
    pthread_t thread_id;
    shared_data_t* shared_data;
    int index;
    int iteration;
} consumer_args_t;


void* producer_function(void* args){
    producer_args_t* producer = (producer_args_t*) args;
    shared_data_t* shared_data = producer->shared_data;
    printf("[Produttore%d] Ciao!\n",producer->index);
    int r_number;

    while(producer->iteration<20){
        sem_wait(&shared_data->empty);
        r_number= rand()%100;
        printf("[Produttore%d] ho generato il numero %d\n",producer->index,r_number);
        pthread_mutex_lock(&shared_data->mutex);
        shared_data->buffer[shared_data->buffer_tail]=r_number;
        shared_data->buffer_tail= (shared_data->buffer_tail +1 ) % BUFFER_SIZE;
        producer->iteration= producer->iteration+1;
        pthread_mutex_unlock(&shared_data->mutex);
        sem_post(&shared_data->full);
    }

    printf("[Produttore%d] Ho finito le iterazioni!\n",producer->index);
    pthread_exit(NULL);

}

void* consumer_funtion(void* args){
    consumer_args_t* consumer = (consumer_args_t*) args;
    shared_data_t* shared_data = consumer->shared_data;
    printf("[Consumatore%d] Ciao!\n",consumer->index);
    
    while(consumer->iteration<20){
        sem_wait(&shared_data->full);
        pthread_mutex_lock(&shared_data->mutex);
        printf("[Consumatore%d] consumo il valore %d-esimo dal buffer: %d\n",consumer->index,shared_data->buffer_head,shared_data->buffer[shared_data->buffer_head]);
        shared_data->buffer_head= (shared_data->buffer_head +1) %BUFFER_SIZE;
        consumer->iteration ++;
        pthread_mutex_unlock(&shared_data->mutex);
        sem_post(&shared_data->empty);        
    }

    printf("[Consumatore%d] ho finito le iterazioni\n", consumer->index);
    pthread_exit(NULL);

}


void main(int argc, char** argv){

    if(argc<2){
        perror("Errore negli argomenti: specificare il numero di thread da creare");
        exit(EXIT_FAILURE);
    }

    int n_thread=atoi(argv[1]);

    shared_data_t* shared_data = (shared_data_t*) malloc(sizeof(shared_data_t));
    if (shared_data == NULL) {
        perror("Errore nell'allocazione della memoria per shared_data");
        exit(EXIT_FAILURE);
    }
    shared_data->buffer_head=0;
    shared_data->buffer_tail=0;

    if(sem_init(&shared_data->empty,0,BUFFER_SIZE)<0){
        perror("Errore nella creazione di empty");
        exit(EXIT_FAILURE);

    }

    if(sem_init(&shared_data->full,0,0)<0){
        perror("Errore nella creazione di full");
        exit(EXIT_FAILURE);
    }

    if(pthread_mutex_init(&shared_data->mutex,NULL)<0){
        perror("Errore nella creazione del mutex");
        exit(EXIT_FAILURE);
    }

    producer_args_t** producer_args = (producer_args_t**) malloc(sizeof(producer_args_t*)*n_thread);
    consumer_args_t** consumer_args = (consumer_args_t**) malloc(sizeof(consumer_args_t*)*n_thread);

    for(int i=0;i<n_thread;i++){
        producer_args[i]= (producer_args_t*) malloc(sizeof(producer_args_t));
        producer_args[i]->index=i;
        producer_args[i]->iteration=0;
        producer_args[i]->shared_data=shared_data;
        
        if(pthread_create(&producer_args[i]->thread_id,NULL,producer_function,producer_args[i])<0){
            perror("Errore nella creazione dei thread producer");
            exit(EXIT_FAILURE);
        }

    }
    
    for(int i=0;i<n_thread;i++){
        consumer_args[i]= (consumer_args_t*) malloc(sizeof(consumer_args_t));
        consumer_args[i]->index=i;
        consumer_args[i]->iteration=0;
        consumer_args[i]->shared_data=shared_data;
        if(pthread_create(&consumer_args[i]->thread_id, NULL, consumer_funtion,consumer_args[i])<0){
            perror("Errore nella creazione dei thread consumer");
            exit(EXIT_FAILURE);
        }       

    }

    for(int i=0;i<n_thread;i++){
        pthread_join(producer_args[i]->thread_id,NULL);
        pthread_join(consumer_args[i]->thread_id,NULL);
    }


}