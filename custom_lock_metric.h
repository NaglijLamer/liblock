#ifndef _CUSTOM_LOCK_METRICS_H
#define _CUSTOM_LOCK_METRICS_H

#include "metrics.h"

#define pthread_spin_lock_c pthread_spin_lock_metric_c
#define pthread_spin_unlock_c pthread_spin_unlock_metric_c
#define pthread_spin_init_c pthread_spin_init_metric_c
#define pthread_spinlock_t_c pthread_spinlock_metric_t_c

#define pthread_mutex_lock_c pthread_mutex_lock_metric_c
#define pthread_mutex_unlock_c pthread_mutex_unlock_metric_c
#define pthread_mutex_init_c pthread_mutex_init_metric_c
#define pthread_mutex_t_c pthread_mutex_metric_t_c

/*Probably must be removed, cause it is not a great thing*/
#define pthread_mutex_lock pthread_mutex_lock_metric
#define pthread_mutex_unlock pthread_mutex_unlock_metric
#define pthread_mutex_init pthread_mutex_init_metric
#define pthread_mutex_t pthread_mutex_metric_t

/*And this too*/
#define pthread_spin_lock pthread_spin_lock_metric
#define pthread_spin_unlock pthread_spin_unlock_metric
#define pthread_spin_init pthread_spin_init_metric
#define pthread_spinlock_t pthread_spinlock_metric_t

#define ticket_spin_lock ticket_spin_lock_metric
#define ticket_spin_unlock ticket_spin_unlock_metric
#define ticket_spin_init ticket_spin_init_metric
#define ticket_spinlock_t ticket_spinlock_metric_t

#define adaptive_mutex_lock adaptive_mutex_lock_metric
#define adaptive_mutex_unlock pthread_mutex_unlock_metric_c
#define adaptive_mutex_init pthread_mutex_init_metric_c
#define adaptive_mutex_t pthread_mutex_metric_t_c

#define yield_mutex_lock pthread_mutex_lock_metric_c
#define yield_mutex_unlock yield_mutex_unlock_metric
#define yield_mutex_init pthread_mutex_init_metric_c
#define yield_mutex_t pthread_mutex_metric_t_c

#define MCS_spin_lock MCS_spin_lock_metric
#define MCS_spin_unlock MCS_spin_unlock_metric
#define MCS_spin_init MCS_spin_init_metric
#define MCS_lock_t MCS_lock_metric_t

#ifdef _SPINCLASSIC
	#include "pthread_spin_custom_metric.h"
        #define custom_lock_lock pthread_spin_lock_metric_c
        #define custom_lock_unlock pthread_spin_unlock_metric_c
        #define custom_lock_init pthread_spin_init_metric_c
        //#define custom_lock pthread_spinlock_metric_t_c
#elif defined _TICKETSPIN
        #include "ticket_spin_metric.h"
        #define custom_lock_lock ticket_spin_lock_metric
        #define custom_lock_unlock ticket_spin_unlock_metric
        #define custom_lock_init ticket_spin_init_metric
        //#define custom_lock ticket_spinlock_t
#elif defined(_MUTEX) || defined (_ADAPTIVE) || defined (_YIELD)
        #include "pthread_mutex_custom_metric.h"
        int temp_var_custom_lock_for_mutex_attr = 1;
        #ifdef _MUTEX
                #define custom_lock_lock pthread_mutex_lock_metric_c
		#define custom_lock_unlock pthread_mutex_unlock_metric_c
        #elif _ADAPTIVE
                #include "adaptive_mutex_metric.h"
                #define custom_lock_lock adaptive_mutex_lock_metric
		#define custom_lock_unlock pthread_mutex_unlock_metric_c
	#else 
		#include "yield_mutex_metric.h"
		#define custom_lock_lock pthread_mutex_lock_metric_c
		#define custom_lock_unlock yield_mutex_unlock_metric
        #endif
        #define custom_lock_init(lock, attr) pthread_mutex_init_metric_c(lock, attr == 1 ? (void*)(&temp_var_custom_lock_for_mutex_attr) : NULL)
        //#define custom_lock pthread_mutex_t_c
#elif defined _MCS_SPIN
        #include "MCS_spin_metric.h"
        #define custom_lock_lock MCS_spin_lock_metric
        #define custom_lock_unlock MCS_spin_unlock_metric
        #define custom_lock_init MCS_spin_init_metric
        //#define custom_lock MCS_lock_t
#elif defined _POSIXSPIN
        #include <pthread.h>
	#include "pthread_spin_metric.h"
        #define custom_lock_lock pthread_spin_lock_metric
        #define custom_lock_unlock pthread_spin_unlock_metric
        #define custom_lock_init pthread_spin_init_metric
        //#define custom_lock pthread_spinlock_t
#else
        //#include <pthread.h>
	#include "pthread_mutex_metric.h"
        #define custom_lock_lock pthread_mutex_lock_metric
        #define custom_lock_unlock pthread_mutex_unlock_metric
        #define custom_lock_init(lock, val) pthread_mutex_init_metric(lock, NULL)
        //#define custom_lock pthread_mutex_t
#endif

#endif
