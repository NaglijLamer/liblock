#ifndef _YIELD_MUTEX_METRIC_H
#define _YIELD_MUTEX_METRIC_H

#include "pthread_mutex_custom_metric.h"

typedef pthread_mutex_metric_t_c yield_mutex_metric_t;

int yield_mutex_unlock_metric(pthread_mutex_metric_t_c *lock);
#define yield_mutex_lock_metric pthread_mutex_lock_metric_c
#define yield_mutex_init_metric pthread_mutex_init_metric_c

#endif
