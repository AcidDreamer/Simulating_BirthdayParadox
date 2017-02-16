#include <stdio.h>
#include <pthread.h> 
#include <time.h>

#define NUMBER_OF_INDIVIDUALS  23
#define DAYS 366  //Days you want + 1
#define HOW_MANY_TIMES 10  //This number willl be multiplied by 10
/*Global counter,counting the number of matches*/
static long happened_counter = 0 ;
/*Global counter , counting the number of times we simulated the event*/
static long simulation_counter = 0 ;
/*Mutexes to lock the resources */
static  pthread_mutex_t happenedMutex = PTHREAD_MUTEX_INITIALIZER;          
static  pthread_mutex_t simulationMutex = PTHREAD_MUTEX_INITIALIZER;          

void *count();
void *farm();

int main(){
    int times_run ;
    /*Repeat for HOW_MANY_TIMES*/
    for(times_run = 0;times_run<HOW_MANY_TIMES;times_run++ ){
        /*Create a farm */
        pthread_t farmThread;
        pthread_create(&farmThread, NULL, farm, NULL);     
        pthread_join(farmThread,NULL);
    }
    /*So some results*/
    printf("My result : %ld at %ld tries\n",happened_counter,simulation_counter);
    return 0;
}

/*Create threads to ease the load*/
void *farm(){
    int i ;
    /*create 10 farms , HOW_MANY_TIMES*10  */
    for(i=0;i<20;i++){
        pthread_t countThread;
        pthread_create(&countThread, NULL, farm, NULL);           // creating a thread to handle server messages
        pthread_join(countThread,NULL);
    }
    pthread_exit(NULL);
}

/*Create threads to ease the load*/
void *count(){
    pthread_mutex_lock(&happenedMutex);
    simulation_counter++; //we are emulating 1 event
    pthread_mutex_unlock(&happenedMutex);
    srand(time(NULL) + clock()+ getpid()); //Create a random seed
    int table[NUMBER_OF_INDIVIDUALS];
    int i,j;
    /*Fill the table with the number of individuals with a random day of birth */
    for(i<0;i<NUMBER_OF_INDIVIDUALS;i++){
        table[i] = rand()% 365;
    }
    /*Check for possible matches */
    for(i<0;i<NUMBER_OF_INDIVIDUALS;i++){
        for(j<0;j<NUMBER_OF_INDIVIDUALS;j++){
            if(table[i]==table[j]){
                pthread_mutex_lock(&happenedMutex);
                happened_counter++;
                pthread_mutex_unlock(&happenedMutex);
            }
        }
    }
    pthread_exit(NULL);
}
