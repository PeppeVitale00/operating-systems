/*
Scrivi un programma che conti le parole presenti in un file di testo. In particolare, un thread deve essere preposto all'apertura
del file, uno al conteggio e uno alla chiusura.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_opened = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_counted = PTHREAD_COND_INITIALIZER;

FILE *file = NULL;
int word_count = 0;
int file_opened = 0;
int count_done = 0;

void* open_file(void* arg) {
    const char *filename = (const char *)arg; 

    pthread_mutex_lock(&mutex);
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Errore nell'apertura del file");
        exit(EXIT_FAILURE);
    }
    file_opened = 1;
    pthread_cond_signal(&cond_opened);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void* count_words(void* arg) {
    pthread_mutex_lock(&mutex);
    while (!file_opened) {
        pthread_cond_wait(&cond_opened, &mutex);
    }

    char c;
    int in_word = 0;
    while ((c = fgetc(file)) != EOF) {
        if (isspace(c)) {
            if (in_word) {
                in_word = 0;
                word_count++;
            }
        } else {
            in_word = 1;
        }
    }
    if (in_word) {
        word_count++;
    }

    count_done = 1;
    pthread_cond_signal(&cond_counted);
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

void* close_file(void* arg) {
    pthread_mutex_lock(&mutex);
    while (!count_done) {
        pthread_cond_wait(&cond_counted, &mutex);
    }

    fclose(file);
    file = NULL;

    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nome_file>\n", argv[0]);
        return 1;
    }

    pthread_t open_thread, count_thread, close_thread;

    pthread_create(&open_thread, NULL, open_file, (void *)argv[1]);
    pthread_create(&count_thread, NULL, count_words, NULL);
    pthread_create(&close_thread, NULL, close_file, NULL);

    pthread_join(close_thread, NULL);
    pthread_join(open_thread, NULL);
    pthread_join(count_thread, NULL);

    printf("Numero di parole nel file: %d\n", word_count);

    return 0;
}
