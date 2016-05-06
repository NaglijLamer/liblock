#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
//#include "pthread_spin_custom.h"
#include "custom_lock.h"

#define SIZE 26
#define FALSE 0
#define TRUE 1
#define SUCCESS 0
#define ERROR -1
#define SLEEP_TIME 1000000
//#define SLEEP_TIME_REG 1000000
//#define SLEEP_TIME_REV 1000000
#define PRINT_INIT 15
#define error(msg) do\
	{\
		perror(msg);\
		exit(EXIT_FAILURE);\
	}while(FALSE)\

char alphabet[SIZE];
custom_lock lock;
int reg = 0;
int ord = 0;
int stop = 0;

void *change_reg()
{	
	char *ch;
	while (TRUE)
	{
		if (stop) return NULL;
		if (custom_lock_lock(&lock) != SUCCESS)
                        error("threads6:pthread_mutex_lock");
		ch = alphabet;
		while (*ch)
		{
			*ch = *ch ^ 0x20;
			ch++;
		}
		for (int i = 0; i < SIZE; i++)
                        printf("%c ", alphabet[i]);
                printf("%c", '\n');
		reg++;
		double m = 1;
		for (int i = 1; i < 1000000; i++)
			m *= i;
		printf("%f\n", m);
		if (custom_lock_unlock(&lock) != SUCCESS)
                        error("threads6:pthread_mutex_unlock");
		//usleep(SLEEP_TIME_REG);
	}
}

void *change_ord()
{	
	char temp;
	int i;
	while (TRUE)
	{
		if (stop) return NULL;
		if (custom_lock_lock(&lock) != SUCCESS)
	                error("threads6:pthread_mutex_lock");
		for (i = 0; i < (SIZE / 2); i++)
		{
			temp = alphabet[i];
			alphabet[i] = alphabet[SIZE - 1 - i];
			alphabet[SIZE - 1 - i] = temp;			
		}
		for (int i = 0; i < SIZE; i++)
                        printf("%c ", alphabet[i]);
                printf("%c", '\n');
		ord++;
		double m = 1;
                for (int i = 1; i < 1000000; i++)
                        m *= i;
		printf("%f\n", m);
		if (custom_lock_unlock(&lock) != SUCCESS)
                        error("threads6:pthread_mutex_unlock");
		//usleep(SLEEP_TIME_REV);
	}
}

int main ()
{
	int i;
	char ch;
	pthread_t ordtr, regtr;
	for (i = 0, ch = 'a'; i < SIZE; i++, ch++)
                alphabet[i] = ch;
	if (custom_lock_init(&lock, 0) != SUCCESS)
		error("threads6:pthread_mutex_init");
	//fprintf(stderr, "N: %ld\n", lock.__N);
	if (pthread_create(&regtr, NULL, &change_reg, NULL) != SUCCESS)
		error("threads6:pthread_create");
	if (pthread_create(&ordtr, NULL, &change_ord, NULL) != SUCCESS)
		error("threads6:pthread_create");
	/*if (pthread_create(&regtr, NULL, &change_reg, NULL) != SUCCESS)
                error("threads6:pthread_create");
        if (pthread_create(&ordtr, NULL, &change_ord, NULL) != SUCCESS)
                error("threads6:pthread_create");*/
	//while (TRUE)
	for (int print = 0; print < PRINT_INIT; print++)
	{
		/*if (custom_lock_lock(&lock) != SUCCESS)
                        error("threads6:pthread_mutex_lock");
		printf("%d: ", print);
		for (i = 0; i < SIZE; i++)
			printf("%c ", alphabet[i]);
		printf("%c", '\n');
		if (custom_lock_unlock(&lock) != SUCCESS)
                        error("threads6:pthread_mutex_unlock");*/
		usleep(SLEEP_TIME);
	}
	stop = 1;
	pthread_join(ordtr, NULL);
	pthread_join(regtr, NULL);
	//fprintf(stderr, "N: %ld\n", lock.__N);
	//fprintf(stderr, "Ord = %d\nReg = %d\nTotal lockings = %d\n", ord, reg, ord + reg);
	return EXIT_SUCCESS;
}
