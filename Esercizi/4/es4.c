//Crea un thread che stampa "Hello, World!" e termini correttamente.

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <syscall.h>

void* hello (void* arg){
    
    printf("Hello World!, sono il thread %ld",syscall(SYS_gettid));

}



int main(){ 

    pthread_t t1;
    int p=32;

    pthread_create(&t1,NULL,hello,NULL);

    pthread_join(t1, NULL);


    return 0;
}