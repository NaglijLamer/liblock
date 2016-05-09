#ifndef _CUSTOM_LOCK_NORMAL_H
#define _CUSTOM_LOCK_NORMAL_H

//#include "global_metric.h"

#ifdef _SPINCLASSIC
	#include "pthread_spin_custom.h"
	#define custom_lock_lock pthread_spin_lock_c
	#define custom_lock_unlock pthread_spin_unlock_c
	#define custom_lock_init pthread_spin_init_c
	#define custom_lock pthread_spinlock_t_c
#elif defined _TICKETSPIN
	#include "ticket_spin.h"
	#define custom_lock_lock ticket_spin_lock
	#define custom_lock_unlock ticket_spin_unlock
	#define custom_lock_init ticket_spin_init
	#define custom_lock ticket_spinlock_t
#elif defined(_MUTEX) || defined (_ADAPTIVE) || defined (_YIELD)
	#include "pthread_mutex_custom.h"
	int temp_var_custom_lock_for_mutex_attr = 1;
	#ifdef _MUTEX
		#define custom_lock_lock pthread_mutex_lock_c
		#define custom_lock_unlock pthread_mutex_unlock_c
	#elif defined _ADAPTIVE
		#include "adaptive_mutex.h"
		#define custom_lock_lock adaptive_mutex_lock
		#define custom_lock_unlock pthread_mutex_unlock_c
	#else
		#include "yield_mutex.h"
		#define custom_lock_lock pthread_mutex_lock_c
		#define custom_lock_unlock yield_mutex_unlock
	#endif
	#define custom_lock_init(lock, attr) pthread_mutex_init_c(lock, attr == 1 ? (void*)(&temp_var_custom_lock_for_mutex_attr) : NULL)
	#define custom_lock pthread_mutex_t_c
#elif defined _MCS_SPIN
	#include "MCS_spin.h"
	#define custom_lock_lock MCS_spin_lock
	#define custom_lock_unlock MCS_spin_unlock
	#define custom_lock_init MCS_spin_init
	#define custom_lock MCS_lock_t
#elif defined _POSIXSPIN
	#include <pthread.h>
	#ifdef _GLOBAL_TIMER_LOCK
	#include "global_metric.h"
	#endif
	#define custom_lock_lock pthread_spin_lock
    	#define custom_lock_unlock pthread_spin_unlock
    	#define custom_lock_init pthread_spin_init
    	#define custom_lock pthread_spinlock_t
#else
	#ifdef _GLOBAL_TIMER_LOCK
	#include "global_metric.h"
	#endif
	#include <pthread.h>
	#define custom_lock_lock pthread_mutex_lock
	#define custom_lock_unlock pthread_mutex_unlock
	#define custom_lock_init(lock, val) pthread_mutex_init(lock, NULL)
	#define custom_lock pthread_mutex_t
#endif

#endif

