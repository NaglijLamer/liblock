#include "pthread_spin_metric.h"
#include <pthread.h>

int pthread_spin_lock_metric(pthread_spinlock_metric_t *lock){
	METRIC_BEFORE_LOCK;

	int res = pthread_spin_lock(lock->spinlock_for_posix);

	METRIC_AFTER_LOCK;
	return res;
}

int pthread_spin_unlock_metric(pthread_spinlock_metric_t *lock){
	METRIC_BEFORE_UNLOCK;
	
	return pthread_spin_unlock(lock->spinlock_for_posix);
}

int pthread_spin_init_metric(pthread_spinlock_metric_t *lock, int ignore){
	METRIC_INIT_LOCK;
	lock->spinlock_for_posix = (pthread_spinlock_t*)malloc(sizeof(pthread_spinlock_t));
	return pthread_spin_init(lock->spinlock_for_posix, ignore);
}
