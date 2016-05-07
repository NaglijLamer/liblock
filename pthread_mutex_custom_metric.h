#ifndef _CUSTOM_MUTEX_METRIC_H
#define _CUSTOM_MUTEX_METRIC_H

#include "metrics.h"
#include "global_metric.h"
#include "metric_function.h"

/*#define SYS_futex 202
#define FUTEX_WAIT 0
#define FUTEX_PRIVATE_FLAG 128
#define FUTEX_WAKE 1*/
	
//typedef volatile int pthread_mutex_t_c;

/*typedef volatile struct{
	int __lock;
	int __spins;
	//METRIC_VARS
} pthread_mutex_t_c;*/
typedef custom_lock pthread_mutex_metric_t_c;

int pthread_mutex_init_metric_c(pthread_mutex_metric_t_c *lock, void *attr);
int pthread_mutex_lock_metric_c(pthread_mutex_metric_t_c *lock);
int pthread_mutex_unlock_metric_c(pthread_mutex_metric_t_c *lock);

#endif
