#include <stdio.h>
#include <pthread.h> 
#include <time.h>

#define NUMBER_OF_INDIVIDUALS  23
#define DAYS 366 //Days you want + 1
#define HOW_MANY_TIMES 10//This number willl be multiplied by 10
/*Global counter,counting the number of matches*/
static long happened_counter = 0 ;
static long combination_counter = 0 ;
void *count();
void *farm();

int main(){
    int times_run ;
    for(times_run =0;times_run<HOW_MANY_TIMES;times_run++ ){
        pthread_t farmThread;
        pthread_create(&farmThread, NULL, farm, NULL);          
    }
    printf("My result : %ld at %ld tries\n",&happened_counter,&combination_counter);
    return 0;
}
/*Create threads to ease the load*/
void *count(){
    combination_counter++;
    int table[NUMBER_OF_INDIVIDUALS];
    int i,j;
    for(i<0;i<NUMBER_OF_INDIVIDUALS;i++){
        table[i] = 1 + (int) (10.0 * (rand() / (DAYS + 1.0)));
    }
    for(i<0;i<NUMBER_OF_INDIVIDUALS;i++){
        for(j<0;j<NUMBER_OF_INDIVIDUALS;j++){
            if(table[i]==table[j]) happened_counter++;
        }
    }
    sleep(1);
}
/*Create threads to ease the load*/
void *farm(){
    int i ;
    for(i=0;i<10;i++){
        pthread_t countThread;
        pthread_create(&countThread, NULL, farm, NULL);           // creating a thread to handle server messages
    }
}
