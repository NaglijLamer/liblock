#ifndef _CUSTOM_SPINLOCK_H
#define _CUSTOM_SPINLOCK_H

//#include "metrics.h"

#ifdef _GLOBAL_TIMER_LOCK
#include <time.h>
#include "global_metric.h"
#endif
	
typedef volatile struct {
	int __lock;
	//METRIC_VARS
} pthread_spinlock_t_c;
//typedef volatile int pthread_spinlock_t_c;

int pthread_spin_init_c(pthread_spinlock_t_c *lock, int ignore);
int pthread_spin_lock_c(pthread_spinlock_t_c *lock);
int pthread_spin_unlock_c(pthread_spinlock_t_c *lock);

#endif
