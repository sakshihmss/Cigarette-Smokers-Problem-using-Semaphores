#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

sem_t smoker_waits[3];
sem_t agent;

typedef void *(*smokers_t)(void *);

void* smoker_0(void* arg){
	while(true){
		nanosleep((struct timespec[]){{0, rand() % 200000000}}, NULL);
		printf("Smoker zero is waiting to smoke(has paper)\n");
		sem_wait(&smoker_waits[0]);
		printf("Smoker zero received matches and tobacco and is now making cigarette.\n");
		sem_post(&agent);
	}
	return NULL;
}

void* smoker_1(void* arg){
	while(true){
		nanosleep((struct timespec[]){{0, rand() % 200000000}}, NULL);
		printf("Smoker one is waiting to smoke(has matches)\n");
		sem_wait(&smoker_waits[1]);
		printf("Smoker one received paper and tobacco and is now making cigarette.\n");
		sem_post(&agent);
	}
	return NULL;
}

void* smoker_2(void* arg){
	while(true){
		nanosleep((struct timespec[]){{0, rand() % 200000000}}, NULL);
		printf("Smoker two is waiting to smoke(has tobacco)\n");
		sem_wait(&smoker_waits[2]);
		printf("Smoker two received paper and matches and is now making cigarette.\n");
		sem_post(&agent);
	}
	return NULL;
}

int main(int argc, char* arvg[]){
	srand(time(NULL));
	sem_init(&agent, 0, 1);
	pthread_t smoker_threads[3];
	smokers_t smoker_functions[3] = {smoker_0, smoker_1, smoker_2};

	for (int i=0;i<3;i++){
		sem_init(&smoker_waits[i], 0, 0);
		if (pthread_create(&smoker_threads[i], NULL, smoker_functions[i], NULL) == EAGAIN){
			perror("Insufficient resources to create thread.");
			return 0;
		}
	}

	while(true){
		sem_wait(&agent);
		sem_post(&smoker_waits[rand()%2]);
	}

	for (int i=0;i<3;i++){
		pthread_join(smoker_threads[i], NULL);
	}
}