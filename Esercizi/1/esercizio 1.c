//Scrivere un programma C che legga un array di numeri interi da input e calcoli la loro somma.

#include <stdio.h>

int main() {

    int array[10];
    int sum=0;
    for (int i=0;i<10;i++){
        printf("Inseririsci il numero %d \n", i+1);
        scanf("%d", &array[i]);
        sum+=array[i];
    }

    printf("La somma di tutti i numeri e: %d", sum);

   return 0;

      

}