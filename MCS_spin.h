#ifndef _MCS_SPIN_H
#define _MCS_SPIN_H

#include <pthread.h>
//#include "metrics.h"

#ifdef _GLOBAL_TIMER_LOCK
#include "global_metric.h"
#endif

typedef struct MCS_node MCS_node;
struct MCS_node{
	MCS_node *next;
	//char locked;
	int locked;
};

typedef volatile struct{
	pthread_key_t key;
	MCS_node *L;
	//METRIC_VARS
}MCS_lock_t;

//#include "metrics.h"

int MCS_spin_lock(MCS_lock_t *lock);
int MCS_spin_unlock(MCS_lock_t *lock);
int MCS_spin_init(MCS_lock_t *lock, int locked);

#endif
