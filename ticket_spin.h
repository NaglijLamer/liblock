#ifndef _TICKET_SPINLOCK_H
#define _TICKET_SPINLOCK_H

//#include "metrics.h"

#ifdef _GLOBAL_TIMER_LOCK
#include "global_metric.h"
#endif

typedef volatile struct{
	int next;
	int serv;
	//METRIC_VARS
}ticket_spinlock_t;
//typedef volatile int ticket_spinlock_t;

extern int ticket_spin_init(ticket_spinlock_t *lock, int locked);
extern int ticket_spin_lock(ticket_spinlock_t *lock);
extern int ticket_spin_unlock(ticket_spinlock_t *lock);

#endif
