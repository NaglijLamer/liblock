#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include "pthread_spin_custom.h"

#define SIZE 26
#define FALSE 0
#define TRUE 1
#define SUCCESS 0
#define ERROR -1
#define SLEEP_TIME 1000000
#define SLEEP_TIME_REG 1000000
#define SLEEP_TIME_REV 1000000
#define PRINT_INIT 15
#define error(msg) do\
	{\
		perror(msg);\
		exit(EXIT_FAILURE);\
	}while(FALSE)\

char alphabet[SIZE];
pthread_spinlock_t_c lock;
int reg = 0;
int ord = 0;
int stop = 0;

void *change_reg()
{	
	char *ch;
	while (TRUE)
	{
		if (stop) return NULL;
		if (pthread_spin_lock_c(&lock) != SUCCESS)
                        error("threads6:pthread_mutex_lock");
		ch = alphabet;
		while (*ch)
		{
			/*if (islower((int)*ch) != FALSE)
				*ch = (char)toupper((int)*ch);
			else *ch = (char)tolower((int)*ch);*/
			*ch = *ch ^ 0x20;
			ch++;
		}
		reg++;
		if (pthread_spin_unlock_c(&lock) != SUCCESS)
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
		if (pthread_spin_lock_c(&lock) != SUCCESS)
	                error("threads6:pthread_mutex_lock");
		for (i = 0; i < (SIZE / 2); i++)
		{
			temp = alphabet[i];
			alphabet[i] = alphabet[SIZE - 1 - i];
			alphabet[SIZE - 1 - i] = temp;			
		}
		ord++;
		if (pthread_spin_unlock_c(&lock) != SUCCESS)
                        error("threads6:pthread_mutex_unlock");
		//usleep(SLEEP_TIME_REV);
	}
}

int main (int argc, char *argv[])
{
	int i;
	char ch;
	pthread_t ordtr, regtr;
	for (i = 0, ch = 'a'; i < SIZE; i++, ch++)
                alphabet[i] = ch;
	if (pthread_spin_init_c(&lock, 0) != SUCCESS)
		error("threads6:pthread_mutex_init");
	if (pthread_create(&regtr, NULL, &change_reg, NULL) != SUCCESS)
		error("threads6:pthread_create");
	if (pthread_create(&ordtr, NULL, &change_ord, NULL) != SUCCESS)
		error("threads6:pthread_create");
	//while (TRUE)
	for (int print = 0; print < PRINT_INIT; print++)
	{
		if (pthread_spin_lock_c(&lock) != SUCCESS)
                        error("threads6:pthread_mutex_lock");
		printf("%d: ", print);
		for (i = 0; i < SIZE; i++)
			printf("%c ", alphabet[i]);
		printf("%c", '\n');
		if (pthread_spin_unlock_c(&lock) != SUCCESS)
                        error("threads6:pthread_mutex_unlock");
		usleep(SLEEP_TIME);
	}
	stop = 1;
	pthread_join(ordtr, NULL);
	pthread_join(regtr, NULL);
	printf("Ord = %d\nReg = %d\nTotal spin locked = %d\n", ord, reg, ord + reg + PRINT_INIT);
	return EXIT_SUCCESS;
}
