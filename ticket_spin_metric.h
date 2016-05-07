#ifndef _TICKET_SPINLOCK_METRIC_H
#define _TICKET_SPINLOCK_METRIC_H

#include "metrics.h"
#include "global_metric.h"
#include "metric_function.h"

/*typedef volatile struct{
	int next;
	int serv;
	//METRIC_VARS
}ticket_spinlock_t;*/
//typedef volatile int ticket_spinlock_t;
typedef custom_lock ticket_spinlock_metric_t;

int ticket_spin_init_metric(ticket_spinlock_metric_t *lock, int locked);
int ticket_spin_lock_metric(ticket_spinlock_metric_t *lock);
int ticket_spin_unlock_metric(ticket_spinlock_metric_t *lock);

#endif
