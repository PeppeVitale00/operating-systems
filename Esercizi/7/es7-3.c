/*
Scrivi un programma in C che utilizzi i thread per calcolare la somma degli 
elementi in un array di numeri interi suddiviso in blocchi.
L'array sarà generato casualmente. Ogni thread riceverà un blocco dell'array e
dovrà calcolare la somma degli elementi in quel blocco. Utilizza un mutex
per garantire che l'aggiornamento del contatore totale della somma sia thread-safe.
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


typedef struct {
    int* array;
    int start;
    int end;
    int sum;
}Thread_data;

int total_sum=0;
pthread_mutex_t mutex;

void* sommatoria (void* arg){
    Thread_data* data =(Thread_data*) arg;
    int partial_sum=0;

    for(int i=data->start;i<data->end;i++){
        partial_sum+=data->array[i];
    }
    pthread_mutex_lock(&mutex);
    total_sum+=partial_sum;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);

}


int main(){
    int n;
    int thread_number;
    int* array;
    Thread_data* data;
    pthread_t* threads;

    printf("Inserisci la dimensione dell'array: ");
    scanf("%d", &n);
    array = malloc(n*sizeof(int));
    int prova=0;
    for(int i=0;i<n;i++){
        array[i]=rand()%100;
        printf("Array[%d]: %d\n",i+1, array[i]);
        prova+=array[i];
    }

    printf("inserisci il numero di thread che devono operare: ");
    scanf("%d",&thread_number);
    int segment_size = n/thread_number; 

    printf("Somma main: %d\n", prova);
    

    threads=malloc(thread_number*sizeof(pthread_t));
    data=malloc(thread_number* sizeof(Thread_data));
    

    for(int i=0;i<thread_number;i++){
        data[i].array=array;
        data[i].start=i*segment_size;
        data[i].end=(i==thread_number-1)? n:(i+1)*segment_size;
        pthread_create(&threads[i],NULL, sommatoria,(void*)&data[i]);
    }

    for(int i=0;i<thread_number;i++){
        pthread_join(threads[i],NULL);
        total_sum+=data[i].sum;
    }

    printf("La somma totale è: %d", total_sum);
}
