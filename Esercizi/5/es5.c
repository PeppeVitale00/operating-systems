// Modificare il programma precedente per creare n thread, 
//ciascuno dei quali stampa il proprio identificativo.

#include <stdio.h>
#include <pthread.h>
#include <syscall.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void* thread_function(void* arg) {
    int thread_id = *((int*)arg);
    printf("Hello from thread %d! My thread ID is %lu\n", thread_id+1, pthread_self());
    free(arg);  // Deallocate the memory allocated for the thread_id
    return NULL;
}

int main() {
    int n;
    printf("Enter the number of threads to create: ");
    scanf("%d", &n);

    pthread_t* threads = malloc(n * sizeof(pthread_t));
    if (threads == NULL) {
        perror("Failed to allocate memory for thread IDs");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        int* thread_id = malloc(sizeof(int));  // Allocate memory for thread_id
        if (thread_id == NULL) {
            perror("Failed to allocate memory for thread_id");
            exit(EXIT_FAILURE);
        }
        *thread_id = i;

        if (pthread_create(&threads[i], NULL, thread_function, thread_id) != 0) {
            perror("Failed to create thread");
            free(thread_id);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < n; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

    free(threads);  // Deallocate the memory allocated for threads
    return 0;
}
