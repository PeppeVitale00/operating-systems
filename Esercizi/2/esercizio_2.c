//Scrivere un programma C che utilizzi puntatori per invertire un array di numeri interi.

#include <stdio.h>


int main(){

    int array[] = {1,2,3,4,5,6};
    int length = sizeof(array)/sizeof(array[0]);
    
    int *ptr1,*ptr2;
    
    
    for(int i=0;i<length/2;i++){
        ptr1=array+i;
        ptr2=array+length-1-i;
        int temp;
        temp=*ptr2;
        *ptr2=*ptr1;
        *ptr1=temp;
        ptr2--;
        ptr1++;
        
    }

    for(int i=0;i<length;i++){
        printf("%d - ", array[i]);
    }
}