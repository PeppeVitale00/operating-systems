/*Scrivi un programma che utilizzi due thread. I due thread
 dovranno trovare, all'interno di un array,
il minimo e il massimo per poi stamparli.*/

#include <pthread.h>
#include <stdio.h>

typedef struct min_max
{
    int* array;
    int length;
    int max, min;

}ArrayData;

void* find_min (void* arg){
    ArrayData* data = (ArrayData*)arg;
    data->min=data->array[0];
    for (int i=1;i<data->length;i++){
        if(data->array[i]<data->min)
            data->min=data->array[i];
    }
    pthread_exit(NULL);
    
}

void* find_max (void* arg){
    ArrayData* data = (ArrayData*) arg;
    data->max=data->array[0];
    for (int i=1;i<data->length;i++){
        if(data->array[i]>data->max)
            data->max=data->array[i];
    }
    pthread_exit(NULL);
    
}



int main(){

    pthread_t t_min, t_max;
    ArrayData data;

    int array[] = {10,55,48,99,334,4,673,46,31,38,498,487,64,9,1};
    data.array=array;
    data.length=sizeof(array) / sizeof(array[0]);

    pthread_create(&t_min,NULL,find_min,&data);
    pthread_create(&t_max, NULL, find_max,&data);

    pthread_join(t_min,NULL);
    pthread_join(t_max,NULL);


    return 0;
}