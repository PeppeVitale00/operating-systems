//Implementare un programma C che scansiona una directory e stampa i nomi dei file contenuti

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

void listFile(const char* path){
    struct dirent* entry;

    DIR* ptr = opendir(path);

    if(ptr==NULL) 
        EXIT_FAILURE;
    int i=1;
    while(entry=readdir(ptr)){
        
        printf("%d: %s\n",i,entry->d_name);
        i++;

    }

    closedir(ptr);

}



int main(){

    listFile("/home/giuseppe/Scaricati");



}

