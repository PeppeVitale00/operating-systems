#include <stdio.h>
#include <pthread.h>

// Struttura per passare più argomenti ai thread
typedef struct {
    int num;
    unsigned long long result;
} FactorialData;

// Funzione per calcolare il fattoriale
unsigned long long factorial(int n) {
    if (n == 0) return 1;
    unsigned long long fact = 1;
    for (int i = 1; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

// Funzione eseguita dai thread
void* calculate_factorial(void* arg) {
    FactorialData* data = (FactorialData*)arg;
    data->result = factorial(data->num);
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2;
    FactorialData data1, data2;

    // Inizializzazione dei dati per i due numeri
    data1.num = 5;  // Calcola il fattoriale di 5
    data2.num = 7;  // Calcola il fattoriale di 7

    // Creazione dei thread
    pthread_create(&thread1, NULL, calculate_factorial, &data1);
    pthread_create(&thread2, NULL, calculate_factorial, &data2);

    // Attesa della terminazione dei thread
    pthread_join(thread2, NULL);
    pthread_join(thread1, NULL);

    // Stampa dei risultati
    printf("Il fattoriale di %d è %llu\n", data1.num, data1.result);
    printf("Il fattoriale di %d è %llu\n", data2.num, data2.result);

    return 0;
}
