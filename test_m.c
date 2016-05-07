#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include "custom_lock.h"

#define USAGE "Usage: app_name amount_of_threads total_locks iteractions_in_section iteractions_between_sections"
#define FALSE 0
#define TRUE 1
#define SUCCESS 0
#define ERROR -1
#define SLEEP_TIME 1000000
#define error(msg) do{\
		perror(msg);\
		exit(EXIT_FAILURE);\
	}while(FALSE)\

custom_lock lock;
int count;
int critical_section_iterarions; /*1 000 000*/
int after_section_iterations;

void *thread_function(){
	double m;
	while (TRUE){
		if (custom_lock_lock(&lock) != SUCCESS)
                        error("test_m:custom_lock_lock");
		m = 1.0;
		for (int i = 1; i < critical_section_iterarions; i++)
                        m *= i;
                printf("%f\n", m);
		count--;
                if (custom_lock_unlock(&lock) != SUCCESS)
                        error("test_m:custom_lock_unlock");
		if (count <= 0) return NULL;
		m = 1.0;
		for (int i = 1; i < after_section_iterations; i++)
                        m *= i;
                printf("%f\n", m);
	}
}

int main (int argc, char *argv[]){
	int amount_thr;
	pthread_t *threads;
	if (argc != 5) error("test_m:wrong amount of args");
	if ((amount_thr = atoi(argv[1])) <= 0) error("test_m:wrong first arg");
	if ((count = atoi(argv[2])) <= 0) error("test_m:wrong second arg");
	if ((critical_section_iterarions = atoi(argv[3])) <= 0) error("test_m:wrong third arg");
	if ((after_section_iterations = atoi(argv[4])) <= 0) error("test_m:wrong fourth arg");
	/*fprintf(stderr, "1: %d, 2: %d, 3: %d\n", amount_thr, critical_section_iterarions, after_section_iterations);
	return 0;*/
	if (custom_lock_init(&lock, 0) != SUCCESS)
		error("test_m:custom_lock_init");
	threads = (pthread_t*)malloc(sizeof(pthread_t) * amount_thr);
	for (int i = 0; i < amount_thr; i++)
		if (pthread_create(&(threads[i]), NULL, &thread_function, NULL) != SUCCESS)
                	error("test_m:pthread_create");
	for (int i = 0; i < amount_thr; i++)
		pthread_join(threads[i], NULL);
	return EXIT_SUCCESS;
}
