#ifndef _MUTEX_METRIC_H
#define _MUTEX_METRIC_H

#include "metrics.h"
#include "global_metric.h"

typedef custom_lock pthread_mutex_metric_t;

int pthread_mutex_init_metric(pthread_mutex_metric_t *lock, void *attr);
int pthread_mutex_lock_metric(pthread_mutex_metric_t *lock);
int pthread_mutex_unlock_metric(pthread_mutex_metric_t *lock);

#endif
