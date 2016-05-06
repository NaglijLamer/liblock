#ifndef _MCS_SPIN_METRIC_H
#define _MCS_SPIN_METRIC_H

#include <pthread.h>
#include "global_metric.h"
#include "metrics.h"

//typedef struct MCS_node MCS_node;
struct MCS_node{
	MCS_node *next;
	//char locked;
	int locked;
};

/*typedef volatile struct{
	pthread_key_t key;
	MCS_node *L;
	//METRIC_VARS
}MCS_lock_t;*/

//#include "metrics.h"
typedef custom_lock MCS_lock_metric_t;

int MCS_spin_lock_metric(MCS_lock_metric_t *lock);
int MCS_spin_unlock_metric(MCS_lock_metric_t *lock);
int MCS_spin_init_metric(MCS_lock_metric_t *lock, int locked);

#endif
