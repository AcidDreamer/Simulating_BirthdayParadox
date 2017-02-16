#include <stdio.h>
#include <pthread.h> 
#include <time.h>
#include <unistd.h>

#define NUMBER_OF_INDIVIDUALS  23
#define DAYS 366  //Days you want + 1
/*Global counter,counting the number of matches*/
static long int happened_counter = 0 ;
/*Global counter , counting the number of times we simulated the event*/
static long int simulation_counter = 0 ;
/*Mutexes to lock the resources */
static  pthread_mutex_t happenedMutex = PTHREAD_MUTEX_INITIALIZER;          
static  pthread_mutex_t simulationMutex = PTHREAD_MUTEX_INITIALIZER;          

void *count();
void *print();

int main(){
    int times_run ;
    /*Create a thread to run simulations */
    pthread_t countThread;
    pthread_create(&countThread, NULL, count, NULL);  
    /*Create a thread to show results */
    pthread_t printThread;
    pthread_create(&printThread, NULL, print, NULL);     
    pthread_join(countThread,NULL);

    return 0;
}

/*Create threads to ease the load*/
void *count(){
    for(;;){
        pthread_mutex_lock(&happenedMutex);
        simulation_counter++; //we are emulating 1 event
        pthread_mutex_unlock(&happenedMutex);
        srand(time(NULL) + clock()+ getpid()); //Create a random seed
        int table[NUMBER_OF_INDIVIDUALS];
        int i,j;
        /*Fill the table with the number of individuals with a random day of birth */
        for(i=0;i<NUMBER_OF_INDIVIDUALS;i++){
            table[i] = rand()% 365;
        }
        /*Check for possible matches */
        for(i=0;i<NUMBER_OF_INDIVIDUALS;i++){
            for(j=0;j<NUMBER_OF_INDIVIDUALS;j++){
                if(table[i]==table[j] && i!=j){
                    printf("%lf %lf \n",table[i],table[j]);
                    pthread_mutex_lock(&happenedMutex);
                    happened_counter++;
                    pthread_mutex_unlock(&happenedMutex);
                }
            }
        }
        usleep(10);
    }
    pthread_exit(NULL);
}
void *print(){
    for(;;){
        printf("My result : %ld at %ld tries\n",happened_counter,simulation_counter);
        printf("Percentage = %ld %\n",happened_counter/simulation_counter);

        sleep(5);
    }
}
