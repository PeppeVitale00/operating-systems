/*
Scrivere un programma che prenda in input un file di testo e ne mostri il contenuto
*/

#include <stdio.h>


int main(){

FILE* ptr;
ptr = fopen("filename.txt", "r");

fprintf(ptr,"Popoppero");
char text[100];
fgets(text,100,ptr);

printf("nel file c'è scritto: %s", text);
fclose(ptr);


}