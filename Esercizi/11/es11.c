 //Estendere il programma precedente per calcolare la
//dimensione totale dei file regolari nella directory senza usare thread

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

unsigned long long calculate_size(const char* path){
    unsigned long long total_size=0;
    DIR* directory;
    struct dirent* entry;
    struct stat fileinfo;
    char filepath[1024];

    directory=opendir(path);

    if(directory==NULL){
        perror("Errore nell'apertura della directory");
        EXIT_FAILURE;
    }


    while((entry=readdir(directory))!=NULL){
        if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0){
            continue;
        }

        snprintf(filepath,sizeof(filepath),"%s/%s",path,entry->d_name);

       if(stat(filepath,&fileinfo)==0){
        if(S_ISREG(fileinfo.st_mode))
            total_size+=fileinfo.st_size;
        }
        else{
            perror("stat");
        }
        
        closedir(directory);

        return total_size;
    }

    
}


int main(){

        unsigned long long size =calculate_size("./");


        printf("%lld byte",size);

    }

