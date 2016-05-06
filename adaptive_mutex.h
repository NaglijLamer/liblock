#ifndef _ADAPTIVE_MUTEX_H
#define _ADAPTIVE_MUTEX_H

#include "pthread_mutex_custom.h"

//#ifdef _GLOBAL_TIMER_LOCK
//#include "global_metric.h"
//#endif
typedef pthread_mutex_t_c adaptive_mutex_t;

int adaptive_mutex_lock(pthread_mutex_t_c *mutex);
#define adaptive_mutex_unlock pthread_mutex_unlock_c
#define adaptive_mutex_init pthread_mutex_init_c

#endif
