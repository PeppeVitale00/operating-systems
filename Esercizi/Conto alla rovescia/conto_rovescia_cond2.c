#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct {

    int start;
    sem_t sem;
    
}shared_data_t;

typedef struct {
    shared_data_t* shared_data;
    int index;
    pthread_t thread_id;
}thread_args_t;

void* routine (void* args){
    thread_args_t* thread_arg = (thread_args_t*) args;
    shared_data_t* shared_data = thread_arg->shared_data;

    printf("[T%d] Sono pronto\n",thread_arg->index);

    while(shared_data->start>0){
        sem_wait(&shared_data->sem);
        if(shared_data->start==0){
           sem_post(&shared_data->sem);
           break;
        }
        shared_data->start--;
        printf("[T%d] nuovo valore: %d\n", thread_arg->index,shared_data->start);
        
        sem_post(&shared_data->sem);
        
      
    }

    pthread_exit(NULL);

}




void main(int argc, char** argv){

    if(argc<3){
        perror("Errore nei parametri");
        exit(EXIT_FAILURE);
    }

    int n_threads= atoi(argv[2]);

    shared_data_t* shared_data = (shared_data_t*) malloc (sizeof(shared_data_t));

    shared_data->start=atoi(argv[1]);

    if(sem_init(&shared_data->sem,0,1)<0){
        perror("Errore nella creazione del semaforo");
        exit(EXIT_FAILURE);
    }
    

    thread_args_t** thread_args = (thread_args_t**) malloc(sizeof(thread_args_t*)*n_threads);

    for(int i=0; i<n_threads;i++){
        thread_args[i] = (thread_args_t*) malloc(sizeof(thread_args_t));
        thread_args[i]->index=i;
        thread_args[i]->shared_data = shared_data;
        if(pthread_create(&thread_args[i]->thread_id,NULL,routine,thread_args[i])<0){
            perror("Errore nella creazione dei thread");
            exit(EXIT_FAILURE);
        }
    }
   

    for (int i = 0; i < n_threads; i++)
    {
        pthread_join(thread_args[i]->thread_id,NULL);
    }

    printf("[MAIN] hanno finito tutti!\n");
    

}