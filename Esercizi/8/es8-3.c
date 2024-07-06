/*
Scrivi un programma che conti il numero di occorrenze di vocali presenti all'interno di un file 
di testo. il conteggio delle vocali deve eessere fatto da thread e diversi così come l'apertura 
e la chiusura del file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condizione_apertura = PTHREAD_COND_INITIALIZER;
pthread_cond_t condizione_conteggio = PTHREAD_COND_INITIALIZER;

FILE* ptr=NULL;
int a=0,e=0,i=0,o=0,u=0;
int file_opened=0;
int vowels_counted=0;

void* open(void* arg){
    const char* filename = (const char*)arg;
    pthread_mutex_lock(&mutex);

    ptr=fopen(filename,"r");

    if(ptr==NULL){
        perror("Errore durante l'apertura del file");
        exit(EXIT_FAILURE);
    }
    printf("\nFILE APERTO!\n");
    file_opened=1;
    pthread_cond_signal(&condizione_apertura);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* count_a(void* arg){
    pthread_mutex_lock(&mutex);
    while(!file_opened){
        pthread_cond_wait(&condizione_apertura,&mutex);
    }
    printf("\nCIAO! Sono il thread adesso conto le 'a' presenti nel tuo testo\n");
    char c;
    rewind(ptr);

    while((c=fgetc(ptr))!=EOF){
        if(c=='a' || c=='A'){
            a++;
        }
    }
    vowels_counted=1;
    pthread_cond_signal(&condizione_conteggio);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* count_e(void* arg){
    pthread_mutex_lock(&mutex);
    while(!file_opened){
        pthread_cond_wait(&condizione_apertura,&mutex);
    }
    printf("\nCIAO! Sono il thread adesso conto le 'e' presenti nel tuo testo\n");
    rewind(ptr);
    char c;
    while((c=fgetc(ptr))!=EOF){
        if(c=='e'|| c=='E'){
            e++;
        }
    }
    vowels_counted=1;
    pthread_cond_signal(&condizione_conteggio);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void* close(void *arg){
    pthread_mutex_lock(&mutex);
    while (!vowels_counted)  {
        pthread_cond_wait(&condizione_conteggio,&mutex);
    }
    fclose(ptr);
    ptr=NULL;
    printf("\nFILE CHIUSO!\n");
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);    
}

int main(){

    pthread_t opener,a_counter,e_counter, closer;

    pthread_create(&opener,NULL,open,"filename.txt");
    pthread_create(&a_counter,NULL,count_a,NULL);
    pthread_create(&e_counter,NULL,count_e,NULL);
    pthread_create(&closer,NULL,close,NULL);


    pthread_join(opener,NULL);
    pthread_join(a_counter,NULL);
    pthread_join(e_counter,NULL);
    pthread_join(closer,NULL);

    printf("\nA: %d\nE: %d\n",a,e);




    return 0;
}