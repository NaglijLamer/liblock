#include "pthread_mutex_metric.h"
#include <pthread.h>

int pthread_mutex_lock_metric(pthread_mutex_metric_t *lock){
	METRIC_BEFORE_LOCK;

	int res = pthread_mutex_lock(lock->mutex_for_posix);

	METRIC_AFTER_LOCK;
	return res;
}

int pthread_mutex_unlock_metric(pthread_mutex_metric_t *lock){
	METRIC_BEFORE_UNLOCK;
	
	return pthread_mutex_unlock(lock->mutex_for_posix);
}

int pthread_mutex_init_metric(pthread_mutex_metric_t *lock, void *attr){
	METRIC_INIT_LOCK;
	lock->mutex_for_posix = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	return pthread_mutex_init(lock->mutex_for_posix, attr);
}