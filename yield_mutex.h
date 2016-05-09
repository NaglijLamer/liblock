#ifndef _YIELD_MUTEX_H
#define _YIELD_MUTEX_H

#include "pthread_mutex_custom.h"

//#ifdef _GLOBAL_TIMER_LOCK
//#include "global_metric.h"
//#endif
typedef pthread_mutex_t_c yield_mutex_t;

int yield_mutex_unlock(pthread_mutex_t_c *mutex);
#define yield_mutex_lock pthread_mutex_lock_c
#define yield_mutex_init pthread_mutex_init_c

#endif
