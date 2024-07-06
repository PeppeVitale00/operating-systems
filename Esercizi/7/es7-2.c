/*
Scrivi un programma in C che utilizza i thread per contare il numero di parole in
un testo suddiviso in blocchi. Il testo sarà rappresentato da un array di stringhe.
Ogni thread riceverà un blocco del testo e dovrà contare il numero di parole in quel blocco. Utilizza un mutex
per garantire che l'aggiornamento del contatore totale delle parole sia thread-safe.
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct{
    char** text;
    int start; 
    int end;
} Thread_data;


pthread_mutex_t mutex;
int total_word; //variabile condivisa

void* count(void* arg) {
    Thread_data* data = (Thread_data*) arg;
    int count = 0;

    for (int i = data->start; i < data->end; i++) {
        char* token = strtok(data->text[i], " \t\n");
        while (token != NULL) {
            count++;
            token = strtok(NULL, " \t\n");
        }
    }
    pthread_mutex_lock(&mutex);
    total_word += count;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
   
}


int main() {
    int array_size, num_threads, segment_size;

    printf("Inserisci la dimensione dell'array (numero di righe): ");
    scanf("%d", &array_size);

    // Allocazione dell'array di stringhe
    char** text = (char**) malloc(array_size * sizeof(char*));
    for (int i = 0; i < array_size; i++) {
        text[i] = (char*) malloc(256 * sizeof(char));
        printf("Inserisci la riga %d: ", i + 1);
        scanf(" %[^\n]", text[i]);
    }

    printf("\nInserisci il numero di thread che dovranno operare: ");
    scanf("%d", &num_threads);

    pthread_t threads[num_threads];
    Thread_data data[num_threads];

    segment_size = array_size / num_threads;

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < num_threads; i++) {
        data[i].text = text;
        data[i].start = i * segment_size;
        data[i].end = (i == num_threads - 1) ? array_size : (i + 1) * segment_size;
        pthread_create(&threads[i], NULL, count, (void*) &data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("Numero totale di parole: %d\n", total_word);

    for (int i = 0; i < array_size; i++) {
        free(text[i]);
    }
    free(text);

    return 0;
}
