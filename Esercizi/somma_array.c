/*
Scrivi un programma in C che utiizza due thread per calcolare
la somma degli elementi di un array. Ognithread calcolerà la 
somma di metà dell'array e poi il thread principale sommerà 
i risultati parziali per ottenere la somma totale.

*/

#include <stdio.h>
#include <pthread.h>


typedef struct {
    int lenght;
    int* array;
    int half_sum1,half_sum2;
}ArrayData;

void* calculate_sum1(void* arg){
    ArrayData* data = (ArrayData*)arg;
    data->half_sum1=0;    
    for (int i=0;i<data->lenght/2;i++){
        data->half_sum1+=data->array[i];
    }
    pthread_exit(NULL);
}

void* calculate_sum2(void* arg){
    ArrayData* data= (ArrayData*)arg;
    data->half_sum2=0;
    for (int i=data->lenght/2;i<data->lenght;i++){
        data->half_sum2+=data->array[i];
    }
    pthread_exit(NULL);
}

int main(){
    pthread_t t1,t2;
    ArrayData data;


    int array[] = {1,1,1,1,1,2,2,2,2,2};
    data.array = array;
    data.lenght= sizeof(array)/sizeof(array[0]);
    printf("sizeof=%d", data.lenght);

    pthread_create(&t1,NULL,calculate_sum1,&data);
    pthread_create(&t2,NULL, calculate_sum2, &data);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

    printf("la somma della prima meta e: %d \n",data.half_sum1);
    printf("la somma della seconda meta e: %d",data.half_sum2);

    return 0;
}
