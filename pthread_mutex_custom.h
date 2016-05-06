#ifndef _CUSTOM_MUTEX_H
#define _CUSTOM_MUTEX_H

//#include "metrics.h"

#ifdef _GLOBAL_TIMER_LOCK
#include "global_metric.h"
#endif

/*#define SYS_futex 202
#define FUTEX_WAIT 0
#define FUTEX_PRIVATE_FLAG 128
#define FUTEX_WAKE 1*/
	
//typedef volatile int pthread_mutex_t_c;

typedef volatile struct{
	int __lock;
	int __spins;
	//METRIC_VARS
} pthread_mutex_t_c;

int pthread_mutex_init_c(pthread_mutex_t_c *mutex, void *attr);
int pthread_mutex_lock_c(pthread_mutex_t_c *mutex);
int pthread_mutex_unlock_c(pthread_mutex_t_c *mutex);

#endif
