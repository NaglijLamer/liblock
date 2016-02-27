#ifndef _CUSTOM_LOCK_H
#define _CUSTOM_LOCK_H

//#include "custom_lock.h"
#ifdef _SPINCLASSIC
#include "pthread_spin_custom.h"
typedef pthread_spinlock_t_c custom_lock;
#elif defined _TICKETSPIN
#include "ticket_spin.h"
typedef ticket_spinlock_t custom_lock;
#elif defined _MUTEX
#include "pthread_mutex_custom.h"
typedef pthred_mutex_t_c custom_lock;
#else
#include <pthread.h>
typedef pthread_mutex_t custom_lock;
#endif

typedef union{
	int init_val;
	void *attr;
}init_arg;

__attribute__((always_inline)) extern int custom_lock_lock(custom_lock *lock){
#ifdef _SPINCLASSIC
        return pthread_spin_lock_c(lock);
#elif defined _TICKETSPIN
        return ticket_spin_lock(lock);
#elif defined _MUTEX
        return pthread_mutex_lock_c(lock);
#else
        return pthread_mutex_lock(lock);
#endif
}
	
__attribute__((always_inline)) extern int custom_lock_unlock(custom_lock *lock){
#ifdef _SPINCLASSIC
        return pthread_spin_unlock_c(lock);
#elif defined _TICKETSPIN
        return ticket_spin_unlock(lock);
#elif defined _MUTEX
        return pthread_mutex_unlock_c(lock);
#else
        return pthread_mutex_unlock(lock);
#endif
}

__attribute__((always_inline)) extern int custom_lock_init(custom_lock *lock, init_arg arg){
#ifdef _SPINCLASSIC
        return pthread_spin_init_c(lock, arg.init_val);
#elif defined _TICKETSPIN
        return ticket_spin_init(lock, arg.init_val);
#elif defined _MUTEX
        return pthread_mutex_init_c(lock, arg.init_val == 1 ? &(arg.init_val) : NULL);
#else
        return pthread_mutex_init(lock, NULL);
#endif
}

#endif
