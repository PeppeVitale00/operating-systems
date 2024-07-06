/*
Creare una struttura dati per rappresentare 
uno studente (nome, età, matricola) e scrivere
un programma che ne gestisca un array.
*/

#include <stdio.h>
#include <string.h>

#define MAX_LENGHT 50

struct studente
{
    char nome[MAX_LENGHT];
    int eta, matricola;
};



int main(){
    struct studente s[5];

    for (int i=0;i<5;i++){
        printf("Inserisci il nome dello studente %d\n", i+1);
        fgets(s[i].nome,MAX_LENGHT,stdin); //1  

        // 2

        printf("Inserisci l'eta dello studente %d\n", i+1);
        scanf("%d", &s[i].eta);

        printf("Inserisci la matricola dello studente %d\n", i+1);
        scanf("%d", &s[i].matricola);

        while (getchar() != '\n'); // 3 

    }


    for (int i=0;i<5;i++){
        printf("%d) Nome: %s \n",i+1, s[i].nome);
        printf("Eta: %d",s[i].eta );
        printf("Matricola: %d \n", s[i].matricola);

    }



}



/*

1) fgets permette di leggere delle stringhe tenendo in considerazione gli spazi. 
Legge dallo stdin, un numero massimo di MAX_LENGHT-1 caratteri, tenendo in considerazione
 il \n messo alla fine e lo mette all'interno di s[i].


2)size_t len = strlen(s[i].nome);
        if (len > 0 && s[i].nome[len - 1] == '\n') {
            s[i].nome[len - 1] = '\0';
        }
  in questa riga, in teoria, andrebbe questo codice. Serve per rimuovere il newline dalla fine della stringa per 
  evitare possibili problemi con la lettura delle stringhe successive. Funziona bene anche senza.


3) Questa riga permette di pulire il buffer prima di inserire un nuovo nome. Se questa riga non viene messa, viene chiesto
l'inserimento del nome del primo studente, ma i successivi non verranno inseriti. Infatti, dopo la stampa, avremo 
solo il nome del primo nome inserito e successivamente saranno tutti vuoti.
          
*/