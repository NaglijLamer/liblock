#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "pthread_mutex_custom.h"
#include <unistd.h>
#define SUCCESS 0
#define count 8

pthread_mutex_t_c mutex;
FILE *file;
int lol = 0;
typedef struct{
	pthread_t *thr;
	int work;
}thread;

void crash(char *text, int worker){
	printf("Place is %s with worker %d", text, worker);
	exit(1);
}


void *thread_worker(void *info){
	thread *worker = (thread*)info;
	printf ("Worker %d has started!\n", worker->work);
	while(1){
		if (pthread_mutex_lock_c(&mutex) != SUCCESS) 
			crash("mutex_lock", worker->work);
		printf("This is worker %d. Mutexstate is %d\n", worker->work, mutex);
		fflush(stdout);
		//sleep(1);
		/*printf("This is worker %d. Mutex state is %d, now it will be free.\n", worker->work, mutex);*/
		lol = worker->work;
		sleep(1);
		fprintf(file, "This is worker %d, var is %d. Mutex state is %d. now it wiil be free.\n", worker->work, lol, mutex);
		fflush(file);
		if (pthread_mutex_unlock_c(&mutex) != SUCCESS)
			crash("mutex_unlock", worker->work);
		sleep(1);
		//sleep(2);
	}
	return NULL;
}

int main(){
	printf("Start!\n");
	int temp = 1;
	file = fopen("file", "w");
	pthread_mutex_init_c(&mutex, &temp);
	thread *threads = (thread*)malloc(sizeof(thread) * count);
	printf("Create threads now\n");
	fflush(stdout);
	for (int i = 1; i <= count; i++){
		threads[i - 1].work = i;
		if (pthread_create((pthread_t*)(&threads[i - 1]), NULL, &thread_worker, &threads[i - 1]) != SUCCESS)
			crash("thread_create", i-1);
	}
	//while(1); /*sleep(1000);*/
	sleep(1);
	printf("Open the gate!");
	pthread_mutex_unlock_c(&mutex);
	pthread_join((pthread_t)(*(threads[0].thr)), NULL);
	fclose(file);
	return EXIT_SUCCESS;
}
