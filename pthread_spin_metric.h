#ifndef _SPIN_METRIC_H
#define _SPIN_METRIC_H

#include "metrics.h"
#include "global_metric.h"
#include "metric_function.h"

typedef custom_lock pthread_spinlock_metric_t;

int pthread_spin_init_metric(pthread_spinlock_metric_t *lock, int ignore);
int pthread_spin_lock_metric(pthread_spinlock_metric_t *lock);
int pthread_spin_unlock_metric(pthread_spinlock_metric_t *lock);

#endif
