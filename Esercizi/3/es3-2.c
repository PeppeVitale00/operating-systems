/*

Creare una struttura dati per rappresentare 
uno studente (nome, età, matricola) e scrivere
un programma che ne gestisca un array.
Questa volta, però, dovrai utilizzare un array di puntatori a strutture.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 50

struct studente {
    char nome[MAX_LENGTH];
    int eta, matricola;
};

int main(){

    int n=3;
    struct studente *s[n];

    for (int i = 0; i < n; i++) {
        s[i]=(struct studente*)calloc(1, sizeof(struct studente));
    }

    for(int i=0;i<n;i++){
        printf("Inserisci il nome dello studente %d", i+1);
        fgets(s[i]->nome, MAX_LENGTH,stdin);

        size_t len = strlen(s[i]->nome);
        if (len > 0 && s[i]->nome[len - 1] == '\n') {
            s[i]->nome[len - 1] = '\0';
        }

        printf("Inserisci l'eta dello studente %d",i+1);
        scanf("%d", &s[i]->eta);

        printf("Inseririsci la matricola dello studente %d", i+1);
        scanf("%d", &s[i]->matricola);

        while (getchar() != '\n');


    }
        


           
    
    
}



