#include <stdio.h>
#include <stdlib.h>

int main(){
    int n=10;
    char** parole =(char**) malloc(n* sizeof(char*));

    for (int i = 0; i < n; i++) {
        parole[i] = (char*)malloc(sizeof(char)); // Allocazione per ogni stringa
        printf("Inserisci parola (%d/%d): \n",i+1,n);
        scanf("%s",parole[i]);
    }

    for(int i=0;i<n;i++){        
            printf("%s - ", parole[i]);
        
    }


}
    


  