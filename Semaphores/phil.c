#include "phil.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>
#define LEFTFORK id
#define RIGHTFORK (id + 1)%NUM_PHILOSOPHERS
#define EATING 0
#define THINKING 1
#define CHANGING 2
#define down 10

// the state of each philospher
int state[NUM_PHILOSOPHERS] = {CHANGING,CHANGING,CHANGING,CHANGING,CHANGING};
int forkState[NUM_PHILOSOPHERS] = {down,down,down,down,down};

// we need a semaphore to lock the proccess
// when taking forks because we might run into
// for printing and locking up
// both the pick up and put down forks
sem_t printing;

// locking up the forks
sem_t forks[NUM_PHILOSOPHERS];
int cycles = 0;

// the life of a philos
void* philosphers(void *ids)
{
	int i = 0;
	// cylcing
	int id = *(int *)ids;
	for (i = 0; i < cycles; i++)
	{
		printStatus();
		take_forks(id);
		eating(id);
		put_forks(id);
		thinking(id);
	}
	return NULL;
}

// lock the process of taking the forks
// soo the next threads run this proccess until
// after the current proccess finishes assinging the forks
// the thread
void take_forks(int id)
{
	state[id] = CHANGING;
	printStatus();
	// even number philospher pick up there
	// right fork first
	// then there left fork
	if (id % 2 == 0)
	{
		right_fork(id);
		printStatus();

		left_fork(id);
		printStatus();
	}
	// odd number philosphers should pick up there
	// left fork then there right fork
	else
	{
		left_fork(id);
		printStatus();


		right_fork(id);
		printStatus();
	}
}

// put forks should return the forks
// and should put the process in the thinking state
void put_forks(int id)
{
	state[id] = CHANGING;
	printStatus();

	// up both of the works
	// putting them down

	// i dont think it would matter
	// which fork you release
	
	forkState[LEFTFORK] = down;
	sem_post(&forks[LEFTFORK]);
	printStatus();	

	forkState[RIGHTFORK] = down;
	sem_post(&forks[RIGHTFORK]);
	printStatus();


}
void eating(int id)
{
	state[id] = EATING;
	printStatus();
	// call printing here
	dawdle();
}
void left_fork(int id){
	 sem_wait(&forks[LEFTFORK]);
         forkState[LEFTFORK] = id;
		
} 
void right_fork(int id){
       sem_wait(&forks[RIGHTFORK]);
      forkState[RIGHTFORK] = id;

}
void thinking(int id)
{
	state[id] = THINKING;
	printStatus();
	// call printing here
}
void printStatus()
{
      sem_wait(&printing);
    int i,k = 0;
	printf("| ");
	for (i = 0; i < NUM_PHILOSOPHERS; i++)
	{
		for (k = 0; k < NUM_PHILOSOPHERS; k++)
		{
			if (forkState[k] == i)
			{
				printf("%d", k);
			}
			else
			{
				printf("-");
			}
		}
		switch (state[i])
		{
		case EATING:
			printf(" EAT   |");
			break;
		case THINKING:
			printf(" THINK |");
			break;
		// the thinking state
		default:
			printf("       |");
			break;
		}
	}
	printf("\n");
sem_post(&printing);
    
}
void dawdle() { 

   struct timespec tv;
   int msec = (int)(((double)random() / RAND_MAX) * 1000);

   tv.tv_sec = 0;
   tv.tv_nsec = 1000000 * msec;
   if (-1 == nanosleep(&tv,NULL) ) {
      perror("nanosleep");
   }
}
int main(int argc, char *argv[])
{
	// thread ID = philosphers ID
	// thread = philosphers
	int id[NUM_PHILOSOPHERS] = {0,1,2,3,4};
	int i,k,j,res;
	pthread_t phil[NUM_PHILOSOPHERS];
	// the number of forks is equal
	// the number of philosphers

	if(argc > 1){
		// make sure there are arguments
		cycles = atoi(argv[1]);
		if(cycles <= 0){
				cycles = 1; // the deafult number of cycles
		}
	}
	else{
			// if there are not enough areguments
			// set it to the deafult 
			cycles = 1;
	}
	printf("|=============|============|============|");
        printf("============|============|\n");
	printf("|      A      |      B     |     C      |");
	printf("     D      |      E     |\n");
	printf("|=============|============|============|");
        printf("============|============|\n");
	// initalize the semaphores
	for (i = 0; i < 5; i++)
	{
		sem_init(&forks[i], 0, 1);
	}
	// init the print setting and puting down of
	// forks
	sem_init(&printing,0,1);
	//spawn all proccess
	// start all the proccesse
	for (k = 0; k < NUM_PHILOSOPHERS; k++)
	{
       res = pthread_create(&phil[k],NULL, philosphers, (void *)(&id[k]));
	}
	if (res == -1)
	{ 
		exit(-1);
	}
	// same as pid wait
	// wait for all the proccesses to return
	// this allows for all the proccess
	// to run even after main has finished being called
	for (j = 0; j < NUM_PHILOSOPHERS; j++)
	{
		pthread_join(phil[j], NULL);
	}

	// print the footer 
        printf("|=============|============|============|");
        printf("============|============|\n");
    	return 0;
}
// remind all proccess
