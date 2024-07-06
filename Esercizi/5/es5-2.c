#include <stdio.h>
#include <unistd.h>
#include <syscall.h>
#include <pthread.h>
#include <stdlib.h>

void* hello (void* arg){
    printf("Ciao! Sono il thread %ld \n",syscall(SYS_gettid));
    return NULL;
}

int main(){

    int n=5;

    pthread_t* threads = malloc(sizeof(threads)*n);


    for(int i=0;i<n;i++){
        pthread_create(&threads[i],NULL,hello,NULL);
    }

    for(int i=0;i<n;i++){
        pthread_join(threads[i],NULL);
    }

    return 0;


}