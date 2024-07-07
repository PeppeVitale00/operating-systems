/*
Aggiungere thread al programma precedente, dove ogni thread si occupa di una sottodirectory. 
Utilizzare mutex per sincronizzare l'accesso ai risultati.
*/


#include <stdio.h>              // |   
#include <stdlib.h>             // |---> entrambe libreria standard
#include <pthread.h>            //per i thread
#include <dirent.h>             //per il tipo DIR
#include <sys/stat.h>           //per la struttura stat
#include <string.h>             //per la funzioni strcmp e strcpy 

#define PATH_LENGHT 4096

long long total_size=0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
    char path[PATH_LENGHT];

}thread_data;

void* calculate_directory_size(void* arg){
    thread_data* data= (thread_data*) arg;
    DIR* directory;                             //puntatore alla directory
    struct dirent* directory_entry;             //struttura che contiene le entry,quindi il file, della directory che esaminiamo
    struct stat file_attribute;                 //struttura che contine le informazioni specifiche del file o directory che esaminiamo 
    char fullpath[PATH_LENGHT];
    //directory=opendir(data->path);

    if((directory = opendir(data->path)) == NULL){
        perror("Error duringo opening directory");
        pthread_exit(NULL);
    }

    while((directory_entry=readdir(directory))){
        if(strcmp(directory_entry->d_name,".")==0 ||strcmp(directory_entry->d_name,"..")==0)
            continue;
        snprintf(fullpath,PATH_LENGHT,"%s/%s",data->path,directory_entry->d_name);

        if(lstat(fullpath,&file_attribute)==-1){ //lstat(), popola la struct stat con le informazioni relative al file specificato.
            perror("lstat error");
            continue;
        }

        if(S_ISDIR(file_attribute.st_mode)){
            pthread_t thread;
            thread_data new_data;
            strcpy(new_data.path,fullpath);
            pthread_create(&thread,NULL,calculate_directory_size,&new_data);
            pthread_join(thread,NULL);
        }
        else{
            pthread_mutex_lock(&mutex);
            total_size+=file_attribute.st_size;
            pthread_mutex_unlock(&mutex);
        }
    }

    closedir(directory);
    pthread_exit(NULL);
}


int main(){

    pthread_t thread;
    thread_data data;
    char path[PATH_LENGHT]="./";

    strcpy(data.path,path);

    pthread_create(&thread,NULL,calculate_directory_size,&data);
    pthread_join(thread,NULL);

    printf("\nLa dimensione della directory %s e di %lld byte.",path,total_size);

    return 0;
}
