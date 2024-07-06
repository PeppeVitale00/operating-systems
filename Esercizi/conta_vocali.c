#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h> // per la funzione isspace()

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condizione_apertura = PTHREAD_COND_INITIALIZER;
pthread_cond_t condizione_conteggio_a = PTHREAD_COND_INITIALIZER;
pthread_cond_t condizione_conteggio_e = PTHREAD_COND_INITIALIZER;

FILE* ptr = NULL;
int a = 0, e = 0; // contatori per 'a' e 'e'
int file_aperto = 0;
int conteggio_vocali_finito = 0;

void* apri_file(void* arg) {
    const char* filename = (const char*)arg;
    pthread_mutex_lock(&mutex);

    ptr = fopen(filename, "r");
    if (ptr == NULL) {
        perror("Errore durante l'apertura del file");
        exit(EXIT_FAILURE);
    }
    printf("FILE APERTO: %s\n", filename);
    file_aperto = 1;
    pthread_cond_signal(&condizione_apertura);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void* conta_vocali_a(void* arg) {
    pthread_mutex_lock(&mutex);
    while (!file_aperto) {
        pthread_cond_wait(&condizione_apertura, &mutex);
    }
    printf("Thread 'a' inizia a contare le vocali 'a'\n");
    pthread_mutex_unlock(&mutex);

    char c;
    while ((c = fgetc(ptr)) != EOF) {
        if (c == 'a' || c == 'A') {
            pthread_mutex_lock(&mutex);
            a++;
            pthread_mutex_unlock(&mutex);
        }
    }

    pthread_mutex_lock(&mutex);
    conteggio_vocali_finito = 1;
    pthread_cond_signal(&condizione_conteggio_a);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void* conta_vocali_e(void* arg) {
    pthread_mutex_lock(&mutex);
    while (!file_aperto) {
        pthread_cond_wait(&condizione_apertura, &mutex);
    }
    printf("Thread 'e' inizia a contare le vocali 'e'\n");
    pthread_mutex_unlock(&mutex);

    char c;
    while ((c = fgetc(ptr)) != EOF) {
        if (c == 'e' || c == 'E') {
            pthread_mutex_lock(&mutex);
            e++;
            pthread_mutex_unlock(&mutex);
        }
    }

    pthread_mutex_lock(&mutex);
    conteggio_vocali_finito = 1;
    pthread_cond_signal(&condizione_conteggio_e);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void* chiudi_file(void* arg) {
    pthread_mutex_lock(&mutex);
    while (!conteggio_vocali_finito) {
        pthread_cond_wait(&condizione_conteggio_a, &mutex);
        pthread_cond_wait(&condizione_conteggio_e, &mutex);
    }
    fclose(ptr);
    ptr = NULL;
    printf("FILE CHIUSO.\n");
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t thread_apertura, thread_conta_a, thread_conta_e, thread_chiusura;

    // Avvio dei thread
    pthread_create(&thread_apertura, NULL, apri_file, "filename.txt");
    pthread_create(&thread_conta_a, NULL, conta_vocali_a, NULL);
    pthread_create(&thread_conta_e, NULL, conta_vocali_e, NULL);
    pthread_create(&thread_chiusura, NULL, chiudi_file, NULL);

    // Attesa della terminazione dei thread
    pthread_join(thread_apertura, NULL);
    pthread_join(thread_conta_a, NULL);
    pthread_join(thread_conta_e, NULL);
    pthread_join(thread_chiusura, NULL);

    // Stampa dei risultati
    printf("\nNumero di 'a' trovate: %d\n", a);
    printf("Numero di 'e' trovate: %d\n", e);

    return 0;
}
