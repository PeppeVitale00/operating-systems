/*

Scrivi un programma in C che utilizza thread e mutex per calcolare il prodotto scalare di due vettori.
Il prodotto scalare di due vettori A e B di dimensione n è dato dalla somma dei prodotti dei loro elementi 
corrispondenti. Dividi il lavoro tra i thread in modo che ciascun thread calcoli una parte del prodotto scalare 
e aggiorni un risultato condiviso. Utilizza un mutex per sincronizzare l'accesso alla variabile che
contiene il risultato parziale.

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int* a;
    int* b;
    int start;
    int end;
    int partial;
} Thread_data;

int prodotto;
pthread_mutex_t mutex;

void* prodotto_scalare(void* arg){
    Thread_data* data = (Thread_data*) arg;
    
    for (int i=data->start;i<data->end;i++){
        data->partial+=(data->a[i]*data->b[i]);
    }

    pthread_mutex_lock(&mutex);
    prodotto+=data->partial;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}



int main(){

    int* a;
    int* b;
    int n, thread_number;
    Thread_data* data;
    pthread_t* threads;
    pthread_mutex_init(&mutex,NULL);
    printf("Inserisci la dimensione degli array:  ");
    scanf("%d",&n);
    printf("\nInserisci il numero degli array che dovranno operare: ");
    scanf("%d", &thread_number);

    int segment_size=n/thread_number;

    a=malloc(n*sizeof(int));
    b=malloc(n*sizeof(int));
    data=malloc(thread_number* sizeof(Thread_data));
    threads=malloc(thread_number*sizeof(pthread_t));
    long int prodotto_A=1;
    long int prodotto_B=1;
    for (int i=0;i<n;i++){
        a[i]=rand()%10;
        prodotto_A*=a[i];
        b[i]=rand()%10;
        prodotto_B*=b[i];
        printf("A[%d]: %d B[%d]: %d\n",i,a[i],i,b[i]);
    }

    for(int i=0;i<thread_number;i++){
        data[i].a=a;
        data[i].b=b;
        data[i].start=i*segment_size;
        data[i].end=(i==thread_number-1)? n:(i+1)*segment_size;
        pthread_create(&threads[i],NULL,prodotto_scalare,(void*)&data[i]);

    }

    for(int i=0;i<thread_number;i++){
        pthread_join(threads[i],NULL);
        printf("data->partial= %d",data[i].partial);
    }

    printf("\nIl prodotto calcolato dai thread è: %d ",prodotto);
    
}