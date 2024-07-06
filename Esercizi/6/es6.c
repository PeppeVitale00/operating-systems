//Scrivere un programma C che utilizza thread per calcolare la somma
// di elementi in un array dividendo il lavoro tra i thread.


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


typedef struct {
    int* array;
    int start;
    int end;
    int sum;
} Thread_data; 

void* partial_sum(void *arg){
    Thread_data* data = (Thread_data*)arg;
    data->sum=0;
    for(int i=data->start;i<data->end;i++){
        data->sum+= data->array[i];
        printf("Thread %ld ha calcolato una somma parziale di %d\n", pthread_self(), data->sum);
    }

    pthread_exit(NULL);
}


int main(){

    int array_size, threads_number, segment_size;

    
    int* array;
    pthread_t* thread;
    Thread_data* data;


    printf("Inserisci la dimesione dell'array: ");
    scanf("%d",&array_size);
    printf("\nInserisci il numero di thread che dovranno operare: ");
    scanf("%d",&threads_number);

    segment_size=array_size/threads_number;

    //Allocazione della memoria
    data=malloc(threads_number * sizeof(Thread_data));
    thread=malloc(threads_number * sizeof(pthread_t));
    array = malloc(array_size* sizeof(int));
    
    for(int i=0;i<array_size;i++){
        array[i]=rand()%10;
        printf("ARRAY[%d] : %d\n", i,array[i]);
    }

    for(int i=0;i<threads_number;i++){
        data[i].array=array;
        data[i].start=i*segment_size;
        data[i].end=(i==threads_number -1)? array_size :(i+1)*segment_size;
        data[i].sum=0;
        pthread_create(&thread[i], NULL, partial_sum, (void*)&data[i]);
    }
    int total_sum=0;

    for(int i=0;i<threads_number;i++){
        pthread_join(thread[i],NULL);
        total_sum+= data[i].sum;
    }

    printf("la somma totale è: %d",total_sum);

    return 0;


}




