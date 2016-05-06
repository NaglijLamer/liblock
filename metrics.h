#ifndef _METRICS_H
#define _METRICS_H

#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include "common_defs.h"
/*#include <sys/syscall.h>
#include <unistd.h>*/
//#define TIMESPEC_TO_DOUBLE(x) (1000.0 * x.tv_sec + 1e-6 * x.tv_nsec)
	
#define METRIC_VARS long __N;\
	long __Na;\
        double __b;\
        double __a;\
        double __w;\
	double __cpu_w;\
        double __temp_a;\
	double __start_a;\
        double __temp_b;\
	pthread_key_t __key;\
	int __itid;\
	int __count_thr;\
	int *__content_thr;\
        bool __is_next/* = false*/;

//#include "MCS_spin_metric.h"
typedef struct MCS_node MCS_node;

volatile struct{
        int count;
        void **lock;
	int size;
}lock_address;

typedef volatile struct{
	union{
		int __lock;
		int next;
		pthread_key_t key; /*it is an unsigned int*/
	};
	union{
		long __spins;
		long serv;
		MCS_node *L; /*on x64 all pointers are 64 bits*/
		pthread_mutex_t *mutex_for_posix; /*yep, not nice solution*/
		pthread_spinlock_t *spinlock_for_posix; /*again!*/
	};
	METRIC_VARS;
}custom_lock;

#define METRIC_INIT_LOCK ({\
	pthread_key_create((pthread_key_t*)(&((lock)->__key)), NULL);\
	(lock)->__count_thr = 10;\
	(lock)->__content_thr = (int*)malloc(sizeof(int) * (lock)->__count_thr);\
	})

#define METRIC_BEFORE_LOCK double __temp_w, __temp_cpu_w;\
        struct timespec __temp_time, __temp_time_cpu;\
	({\
        (lock)->__N++;\
        clock_gettime(CLOCK_MONOTONIC, &__temp_time);\
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &__temp_time_cpu);\
	__temp_cpu_w = TIMESPEC_TO_DOUBLE(__temp_time_cpu);\
        __temp_w = TIMESPEC_TO_DOUBLE(__temp_time);\
	})

#define METRIC_AFTER_LOCK ({\
	int *n;\
	if ((n = pthread_getspecific((lock)->__key)) == NULL){\
		n = (int*)malloc(sizeof(int));\
                pthread_setspecific((lock)->__key, (void*)n);\
		*n = ((lock)->__itid)++;\
		if ((lock)->__itid >= (lock)->__count_thr){\
			(lock)->__count_thr += 5;\
			(lock)->__content_thr = (int*)realloc((lock)->__content_thr, sizeof(int) * (lock)->__count_thr);\
		}\
		(lock)->__content_thr[*n] = 0;\
	}\
	(lock)->__content_thr[*n]++;\
        clock_gettime(CLOCK_MONOTONIC, &__temp_time);\
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &__temp_time_cpu);\
        (lock)->__temp_b = TIMESPEC_TO_DOUBLE(__temp_time);\
        (lock)->__w += (lock)->__temp_b - __temp_w;\
        (lock)->__cpu_w += TIMESPEC_TO_DOUBLE(__temp_time_cpu) - __temp_cpu_w;\
        /*fprintf(stderr, "TID: %ld, temp_w: %f\n", syscall(SYS_gettid), __temp_w);*/\
        if ((lock)->__is_next) {\
		if (__temp_w >= (lock)->__temp_a) (lock)->__temp_a = __temp_w;\
		else if (__temp_w <= (lock)->__start_a) (lock)->__start_a = __temp_w;\
	}\
        else {\
		(lock)->__start_a = (lock)->__temp_a = __temp_w;\
                (lock)->__is_next = true;\
                lock_address.lock[lock_address.count++] = (void*)(lock);\
                if (lock_address.count >= lock_address.size){\
                        lock_address.size += 20;\
                        lock_address.lock = realloc(lock_address.lock, sizeof(void*) * lock_address.size);\
                }\
	}\
        })


#define METRIC_AFTER_LOCK_OLD ({\
	clock_gettime(CLOCK_MONOTONIC, &__temp_time);\
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &__temp_time_cpu);\
        (lock)->__temp_b = TIMESPEC_TO_DOUBLE(__temp_time);\
        (lock)->__w += (lock)->__temp_b - __temp_w;\
	(lock)->__cpu_w += TIMESPEC_TO_DOUBLE(__temp_time_cpu) - __temp_cpu_w;\
	fprintf(stderr, "TID: %ld, Diff: %f, temp_w: %f, temp_a: %f\n", syscall(SYS_gettid), __temp_w - (lock)->__temp_a, __temp_w, (lock)->__temp_a);\
        if ((lock)->__is_next) {\
		lock->__a += __temp_w - (lock)->__temp_a;\
		lock->__Na++;\
	}\
        else {\
		(lock)->__is_next = true;\
		lock_address.lock[lock_address.count++] = (void*)(lock);\
                if (lock_address.count >= lock_address.size){\
                        lock_address.size += 20;\
                        lock_address.lock = realloc(lock_address.lock, sizeof(void*) * lock_address.size);\
                }\
	}\
        (lock)->__temp_a = __temp_w;\
	})

#define METRIC_BEFORE_UNLOCK ({\
	struct timespec __temp;\
        clock_gettime(CLOCK_MONOTONIC, &__temp);\
        (lock)->__b += TIMESPEC_TO_DOUBLE(__temp) - (lock)->__temp_b;\
	})

#endif
