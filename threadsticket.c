#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include "ticket_spin.h"

#define SIZE 26
#define FALSE 0
#define TRUE 1
#define SUCCESS 0
#define ERROR -1
#define SLEEP_TIME 1000000
#define SLEEP_TIME_REG 1000000
#define SLEEP_TIME_REV 1000000
#define error(msg) do\
	{\
		perror(msg);\
		exit(EXIT_FAILURE);\
	}while(FALSE)\

char alphabet[SIZE];
ticket_spinlock_t lock;
void *change_reg()
{	
	char *ch;
	while (TRUE)
	{
		if (ticket_spin_lock(&lock) != SUCCESS)
                        error("threads6:pthread_mutex_lock");
		ch = alphabet;
		while (*ch)
		{
			if (islower((int)*ch) != FALSE)
				*ch = (char)toupper((int)*ch);
			else *ch = (char)tolower((int)*ch);
			ch++;
		}
		if (ticket_spin_unlock(&lock) != SUCCESS)
                        error("threads6:pthread_mutex_unlock");
		//usleep(SLEEP_TIME_REG);
	}
	return NULL;
}

void *change_ord()
{	
	char temp;
	int i;
	while (TRUE)
	{
		if (ticket_spin_lock(&lock) != SUCCESS)
	                error("threads6:pthread_mutex_lock");
		for (i = 0; i < (SIZE / 2); i++)
		{
			temp = alphabet[i];
			alphabet[i] = alphabet[SIZE - 1 - i];
			alphabet[SIZE - 1 - i] = temp;			
		}
		if (ticket_spin_unlock(&lock) != SUCCESS)
                        error("threads6:pthread_mutex_unlock");
		//usleep(SLEEP_TIME_REV);
	}
	return NULL;
}

int main (int argc, char *argv[])
{
	int i;
	char ch;
	pthread_t ord, reg;
	for (i = 0, ch = 'a'; i < SIZE; i++, ch++)
                alphabet[i] = ch;
	if (ticket_spin_init(&lock, 0) != SUCCESS)
		error("threads6:pthread_mutex_init");
	if (pthread_create(&reg, NULL, &change_reg, NULL) != SUCCESS)
		error("threads6:pthread_create");
	if (pthread_create(&ord, NULL, &change_ord, NULL) != SUCCESS)
		error("threads6:pthread_create");
	while (TRUE)
	{
		if (ticket_spin_lock(&lock) != SUCCESS)
                        error("threads6:pthread_mutex_lock");
		for (i = 0; i < SIZE; i++)
			printf("%c ", alphabet[i]);
		printf("%c", '\n');
		if (ticket_spin_unlock(&lock) != SUCCESS)
                        error("threads6:pthread_mutex_unlock");
		usleep(SLEEP_TIME);
	}
	return EXIT_SUCCESS;
}
