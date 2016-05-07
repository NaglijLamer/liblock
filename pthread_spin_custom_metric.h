#ifndef _CUSTOM_SPINLOCK_METRIC_H
#define _CUSTOM_SPINLOCK_METRIC_H

#include "metrics.h"
#include "global_metric.h"
#include "metric_function.h"
	
/*typedef volatile struct {
	int __lock;
	METRIC_VARS
} pthread_spinlock_metric_t_c;*/
//typedef volatile int pthread_spinlock_t_c;
typedef custom_lock pthread_spinlock_metric_t_c;

int pthread_spin_init_metric_c(pthread_spinlock_metric_t_c *lock, int ignore);
int pthread_spin_lock_metric_c(pthread_spinlock_metric_t_c *lock);
int pthread_spin_unlock_metric_c(pthread_spinlock_metric_t_c *lock);

#endif
