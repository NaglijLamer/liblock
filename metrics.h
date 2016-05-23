#ifndef _METRICS_H
#define _METRICS_H

#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "common_defs.h"
/*#include <sys/types.h>*/
#include <sys/syscall.h>
#include <unistd.h>
#include <sched.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/perf_event.h>
//#define TIMESPEC_TO_DOUBLE(x) (1000.0 * x.tv_sec + 1e-6 * x.tv_nsec)

typedef struct {
	int __count;
	double __section_time;
	double __wait_time;
	double __max_wait_time;
	pid_t __tid;
	int __fd_l;
	int __fd_cr;
	int __fd_out;
	int __fd_ul;
	//int __core;
}__thr;
	
#define METRIC_VARS long __N;\
	/*long __Na;*/\
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
	__thr *__content_thr;\
	/*double __b_max;*/\
	/*pid_t __prev;*/\
	/*double __temp_cpu_b;*/\
	/*double __b_cpu;*/\
	long long __br_l;\
	long long __br_cr;\
	long long __br_out;\
	long long __br_ul;\
        bool __is_next/* = false*/;

//#include "MCS_spin_metric.h"
typedef struct MCS_node MCS_node;

volatile struct{
        int count;
        void **lock;
	int size;
	int fd1;
	int fd2;
	int fd3;
	/*int fd4;
	int fd5;*/
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

#define METRIC_INIT_LOCK(lock) ({\
	pthread_key_create((pthread_key_t*)(&((lock)->__key)), NULL);\
	(lock)->__count_thr = 10;\
	(lock)->__content_thr = (__thr*)malloc(sizeof(__thr) * (lock)->__count_thr);\
	})

#define METRIC_BEFORE_LOCK(lock) double __temp_w, __temp_cpu_w;\
        struct timespec __temp_time, __temp_time_cpu;\
	({\
        /*(lock)->__N++;*/\
        clock_gettime(CLOCK_MONOTONIC, &__temp_time);\
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &__temp_time_cpu);\
	__temp_cpu_w = TIMESPEC_TO_DOUBLE(__temp_time_cpu);\
        __temp_w = TIMESPEC_TO_DOUBLE(__temp_time);\
	\
	/*int *n;*/\
	/*if ((n = pthread_getspecific((lock)->__key)) != NULL){*/\
		/*long long count;*/\
		/*ioctl((lock)->__content_thr[*n].__fd_out, PERF_EVENT_IOC_DISABLE, 0);*/\
		/*if (read((lock)->__content_thr[*n].__fd_out, &count, sizeof(long long)) < 0) exit(1);*/\
		/*(lock)->__br_out += count;*/\
		/*ioctl((lock)->__content_thr[*n].__fd_l, PERF_EVENT_IOC_RESET, 0);*/\
		/*ioctl((lock)->__content_thr[*n].__fd_l, PERF_EVENT_IOC_ENABLE, 0);*/\
	/*}*/\
	})

#define METRIC_AFTER_LOCK(lock) ({\
	int *n;\
	if ((n = pthread_getspecific((lock)->__key)) == NULL){\
		n = (int*)malloc(sizeof(int));\
                pthread_setspecific((lock)->__key, (void*)n);\
		*n = ((lock)->__itid)++;\
		if ((lock)->__itid >= (lock)->__count_thr){\
			(lock)->__count_thr += 5;\
			(lock)->__content_thr = (__thr*)realloc((lock)->__content_thr, sizeof(__thr) * (lock)->__count_thr);\
		}\
		(lock)->__content_thr[*n].__tid = syscall(SYS_gettid);\
		(lock)->__content_thr[*n].__count = 0;\
		\
		/*struct perf_event_attr pe1*//*, pe2, pe3*/;\
		/*memset(&pe1, 0, sizeof(struct perf_event_attr));*/\
		/*memset(&pe2, 0, sizeof(struct perf_event_attr));*/\
		/*memset(&pe3, 0, sizeof(struct perf_event_attr));*/\
		/*pe1.type*/ /*= pe2.type = pe3.type*//* = PERF_TYPE_HARDWARE;*/\
		/*pe1.size*/ /*= pe2.size = pe3.size*//* = sizeof(struct perf_event_attr);*/\
		/*pe1.config = pe2.config = pe3.config = PERF_COUNT_HW_BRANCH_INSTRUCTIONS;*/\
		/*pe1.config*/ /*= pe2.config = pe3.config*//* = PERF_COUNT_HW_BRANCH_MISSES;*/\
		/*pe1.disabled*/ /*= pe2.disabled = pe3.disabled*//* = 1;*/\
		/*(lock)->__content_thr[*n].__fd_cr = syscall(__NR_perf_event_open, &pe1, (lock)->__content_thr[*n].__tid, -1, -1, 0);*/\
		/*(lock)->__content_thr[*n].__fd_l = syscall(__NR_perf_event_open, &pe1, (lock)->__content_thr[*n].__tid, -1, -1, 0);*/\
		/*(lock)->__content_thr[*n].__fd_out = syscall(__NR_perf_event_open, &pe1, (lock)->__content_thr[*n].__tid, -1, -1, 0);*/\
		/*(lock)->__content_thr[*n].__fd_ul = syscall(__NR_perf_event_open, &pe1, (lock)->__content_thr[*n].__tid, -1, -1, 0);*/\
		/*ioctl((lock)->__content_thr[*n].__fd_l, PERF_EVENT_IOC_RESET, 0);*/\
		/*ioctl((lock)->__content_thr[*n].__fd_cr, PERF_EVENT_IOC_RESET, 0);*/\
		/*ioctl((lock)->__content_thr[*n].__fd_out, PERF_EVENT_IOC_RESET, 0);*/\
		/*ioctl((lock)->__content_thr[*n].__fd_ul, PERF_EVENT_IOC_RESET, 0);*/\
		\
		/*(lock)->__content_thr[*n].__core = sched_getcpu();*/\
	}\
	\
	/*long long count;*/\
	/*ioctl((lock)->__content_thr[*n].__fd_l, PERF_EVENT_IOC_DISABLE, 0);*/\
	/*if (read((lock)->__content_thr[*n].__fd_l, &count, sizeof(long long)) < 0) exit(1);*/\
	/*(lock)->__br_l += count;*/\
	\
	(lock)->__N++;\
	/*if ((lock)->__content_thr[*n].__tid == (lock)->__prev) fprintf(stderr, "I am here again at %ld and now %ld. TID: %d\n", (lock)->__N - 1, (lock)->__N, (lock)->__content_thr[*n].__tid);*/\
	/*else (lock)->__prev = (lock)->__content_thr[*n].__tid;*/\
	/*fprintf(stderr, "TID: %d, core: %d\n", (lock)->__content_thr[*n].__tid, sched_getcpu());*/\
	/*int core = sched_getcpu();*/\
	/*if ((lock)->__content_thr[*n].__core != core) {*/\
		/*fprintf(stderr, "%ld: Core migration at tid %d! From core %d to core %d\n", (lock)->__N, (lock)->__content_thr[*n].__tid, (lock)->__content_thr[*n].__core, core);*/\
		/*(lock)->__content_thr[*n].__core = core;*/\
	/*}*/\
	(lock)->__content_thr[*n].__count++;\
        clock_gettime(CLOCK_MONOTONIC, &__temp_time);\
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &__temp_time_cpu);\
        (lock)->__temp_b = TIMESPEC_TO_DOUBLE(__temp_time);\
	double temp = (lock)->__temp_b - __temp_w;\
        (lock)->__w += temp;\
	(lock)->__content_thr[*n].__wait_time += temp;\
	if ((lock)->__content_thr[*n].__max_wait_time < temp) (lock)->__content_thr[*n].__max_wait_time = temp;\
        (lock)->__cpu_w += TIMESPEC_TO_DOUBLE(__temp_time_cpu) - __temp_cpu_w;\
	/*(lock)->__temp_cpu_b = TIMESPEC_TO_DOUBLE(__temp_time_cpu);*/\
	/*(lock)->__cpu_w += (lock)->__temp_cpu_b - __temp_cpu_w;*/\
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
	\
	/*ioctl((lock)->__content_thr[*n].__fd_cr, PERF_EVENT_IOC_RESET, 0);*/\
	/*ioctl((lock)->__content_thr[*n].__fd_cr, PERF_EVENT_IOC_ENABLE, 0);*/\
	\
        })

#define METRIC_AFTER_LOCK_OLD(lock) ({\
	int *n;\
	if ((n = pthread_getspecific((lock)->__key)) == NULL){\
		n = (int*)malloc(sizeof(int));\
                pthread_setspecific((lock)->__key, (void*)n);\
		*n = ((lock)->__itid)++;\
		if ((lock)->__itid >= (lock)->__count_thr){\
			(lock)->__count_thr += 5;\
			(lock)->__content_thr = (__thr*)realloc((lock)->__content_thr, sizeof(__thr) * (lock)->__count_thr);\
		}\
		(lock)->__content_thr[*n].__tid = syscall(SYS_gettid);\
		(lock)->__content_thr[*n].__count = 0;\
	}\
	(lock)->__content_thr[*n].__count++;\
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


#define METRIC_AFTER_LOCK_OLD2 ({\
	clock_gettime(CLOCK_MONOTONIC, &__temp_time);\
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &__temp_time_cpu);\
        (lock)->__temp_b = TIMESPEC_TO_DOUBLE(__temp_time);\
        (lock)->__w += (lock)->__temp_b - __temp_w;\
	(lock)->__cpu_w += TIMESPEC_TO_DOUBLE(__temp_time_cpu) - __temp_cpu_w;\
	/*fprintf(stderr, "TID: %ld, Diff: %f, temp_w: %f, temp_a: %f\n", syscall(SYS_gettid), __temp_w - (lock)->__temp_a, __temp_w, (lock)->__temp_a);*/\
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

#define METRIC_BEFORE_UNLOCK(lock) ({\
	struct timespec __temp/*, __temp_time_cpu*/;\
	\
	/*long long count;*/\
	/*int *n = (int*)pthread_getspecific((lock)->__key);*/\
	/*ioctl((lock)->__content_thr[*n].__fd_cr, PERF_EVENT_IOC_DISABLE, 0);*/\
	/*if (read((lock)->__content_thr[*n].__fd_cr, &count, sizeof(long long)) < 0) exit(1);*/\
	/*(lock)->__br_cr += count;*/\
	/*fprintf(stdout, "%lld\n", count);*/\
	\
        clock_gettime(CLOCK_MONOTONIC, &__temp);\
	/*clock_gettime(CLOCK_THREAD_CPUTIME_ID, &__temp_time_cpu);*/\
	/*(lock)->__b_cpu += TIMESPEC_TO_DOUBLE(__temp_time_cpu) - (lock)->__temp_cpu_b;*/\
        double temp = TIMESPEC_TO_DOUBLE(__temp) - (lock)->__temp_b;\
	(lock)->__b += temp;\
	/*if ((lock)->__b_max < temp) (lock)->__b_max = temp;*/\
	/*fprintf(stderr, "%f\n", temp);*/\
	(lock)->__content_thr[*((int*)pthread_getspecific((lock)->__key))].__section_time += temp;\
	/*int *n = (int*)pthread_getspecific((lock)->__key);*/\
	/*(lock)->__content_thr[*n].__section_time += temp;*/\
	/*int core = sched_getcpu();*/\
	/*if ((lock)->__content_thr[*n].__core != core) {*/\
		/*fprintf(stderr, "%ld: Core migration at tid %d! From core %d to core %d\n", (lock)->__N, (lock)->__content_thr[*n].__tid, (lock)->__content_thr[*n].__core, core);*/\
		/*(lock)->__content_thr[*n].__core = core;*/\
	/*}*/\
	\
		/*ioctl((lock)->__content_thr[*n].__fd_ul, PERF_EVENT_IOC_RESET, 0);*/\
		/*ioctl((lock)->__content_thr[*n].__fd_ul, PERF_EVENT_IOC_ENABLE, 0);*/\
	\
	})

#define METRIC_AFTER_UNLOCK(lock)
#define METRIC_AFTER_UNLOCK2(lock)({\
	\
	long long count;\
	int *n = (int*)pthread_getspecific((lock)->__key);\
	ioctl((lock)->__content_thr[*n].__fd_ul, PERF_EVENT_IOC_DISABLE, 0);\
	if (read((lock)->__content_thr[*n].__fd_ul, &count, sizeof(long long)) < 0) exit(1);\
	(lock)->__br_ul += count;\
	ioctl((lock)->__content_thr[*n].__fd_out, PERF_EVENT_IOC_RESET, 0);\
	ioctl((lock)->__content_thr[*n].__fd_out, PERF_EVENT_IOC_ENABLE, 0);\
	\
	})

#endif
