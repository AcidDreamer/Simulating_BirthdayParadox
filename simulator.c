#include <stdio.h>
#include <pthread.h> 
#include <time.h>
#include <unistd.h>

#define NUMBER_OF_INDIVIDUALS  23 // Number of people
#define DAYS 365  //Days we want to simulate
#define NUMBER_OF_SIMULATIONS 100000 //How many times are we gonna run the simulation
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
    printf("My result : %d at %d tries\n",happened_counter,simulation_counter);
    printf("Percentage = %d %\n",happened_counter/simulation_counter);

    return 0;
}

/*Create threads to ease the load*/
void *count(){
    for(;;){
        if(simulation_counter==NUMBER_OF_SIMULATIONS)break;
        simulation_counter++; //we are emulating 1 event
        srand(time(NULL) + clock()+ getpid()); //Create a random seed
        int table[NUMBER_OF_INDIVIDUALS];
        int i,j;
        /*Fill the table with the number of individuals with a random day of birth */
        for(i=0;i<NUMBER_OF_INDIVIDUALS;i++){
            table[i] =  1 + (rand()% DAYS); //assign a random birthday
        }
        /*Check for possible matches */
        for(i=0;i<NUMBER_OF_INDIVIDUALS;i++){
            for(j=i;j<NUMBER_OF_INDIVIDUALS;j++){
                /*if the values are the same and the i and j aren't the same entry*/
                if(table[i]==table[j] && i!=j){ 
                    happened_counter++;
                }
            }
        }
        usleep(10);
    }
    pthread_exit(NULL);
}
/*To seconds after initialization print the data,then repeat after 5 seconds */
void *print(){
    sleep(2);
    for(;;){
        printf("My result : %d at %d tries\n",happened_counter,simulation_counter);
        printf("Percentage = %d %\n",(happened_counter,simulation_counter);
        sleep(5);
    }
    pthread_exit(NULL);
}
