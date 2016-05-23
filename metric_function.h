#ifdef _METRIC
#ifndef _METRIC_FUNCTION_H
#define _METRIC_FUNCTION_H

#include <stdio.h>
#include <malloc.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <string.h>
#include <locale.h>
#include "metrics.h"

#ifdef _SHORT_METRIC
#define OUTPUT "%p\n%'ld\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n"
#define OUTPUT2 "%d %d %d\n%f\n%f\n%f\n"
#else
#define OUTPUT "Lock address: %p\nAmount of threads wanting to get lock: %ld\nAverage in critical section: %f\nAverage between two threads on lock: %f\nAverage waiting time: %f\nAverage waiting CPU time: %f\nLoading: %f\nAverage time in section+waitng: %f\nAverage queue: %f\nAverage amount of threads in section+waiting: %f\nPer thread locks:\n"
#define OUTPUT2 "Linux TID: %d. Thread internal id on this lock: %d. Amount of attempt to get lock: %d\n\tAverage in critical section: %f\n\tAverage waiting time: %f\n\tMax waiting time: %f\n"
#endif

int in_lock_info_test() __attribute__((constructor(101)));
int out_lock_info_test() __attribute__((destructor(101)));

int in_lock_info_test(){
        lock_address.count = 0;
        lock_address.size = 10;
        lock_address.lock = (void**)malloc(sizeof(void*) * lock_address.size);
	
	pid_t tid = syscall(__NR_gettid);
	//fprintf(stderr, "tid is: %d", tid);
	struct perf_event_attr pe1, pe2, pe3/*, pe4, pe5*/;
	memset(&pe1, 0, sizeof(struct perf_event_attr));
	memset(&pe2, 0, sizeof(struct perf_event_attr));
	memset(&pe3, 0, sizeof(struct perf_event_attr));
	/*memset(&pe4, 0, sizeof(struct perf_event_attr));
	memset(&pe5, 0, sizeof(struct perf_event_attr));*/
	/*memset(&pe4, 0, sizeof(struct perf_event_attr));
	memset(&pe5, 0, sizeof(struct perf_event_attr));*/
	pe1.type = pe2.type = pe3.type /*= pe4.type = pe5.type*/ = PERF_TYPE_HARDWARE;
	pe1.size = pe2.size = pe3.size/* = pe4.size = pe5.size */= sizeof(struct perf_event_attr);
	pe1.config = /*pe4.config = pe5.config = */PERF_COUNT_HW_BRANCH_INSTRUCTIONS;
	//pe1.config = PERF_COUNT_HW_INSTRUCTIONS;
	pe2.config = pe3.config = PERF_COUNT_HW_BRANCH_MISSES;
	pe1.disabled = pe2.disabled = pe3.disabled /*= pe4.disabled = pe5.disabled*/ = 1;
	pe1.inherit = pe2.inherit = pe3.inherit /*= pe4.inherit = pe5.inherit*/ = 1;
	pe1.exclude_kernel = pe2.exclude_kernel /*= pe4.exclude_kernel = pe5.exclude_kernel*/ = 1;
	//pe1.exclude_hv = pe2.exclude_hv = 1;
	/*lock_address.fd1 = syscall(__NR_perf_event_open, &pe1, 0, -1, -1, 0);
	lock_address.fd2 = syscall(__NR_perf_event_open, &pe2, 0, -1, -1, 0);*/
	lock_address.fd1 = syscall(__NR_perf_event_open, &pe1, tid, -1, -1, 0);
	lock_address.fd2 = syscall(__NR_perf_event_open, &pe2, tid, -1, -1, 0);
	lock_address.fd3 = syscall(__NR_perf_event_open, &pe3, tid, -1, -1, 0);
	/*lock_address.fd4 = syscall(__NR_perf_event_open, &pe4, tid, -1, -1, 0);
	lock_address.fd5 = syscall(__NR_perf_event_open, &pe5, tid, -1, -1, 0);*/
	//if (lock_address.fd1 == -1 || lock_address.fd2 == -1) fprintf(stderr, "%s\n", "WAAAAAT");
	ioctl(lock_address.fd1, PERF_EVENT_IOC_RESET, 0);
	ioctl(lock_address.fd2, PERF_EVENT_IOC_RESET, 0);
	ioctl(lock_address.fd3, PERF_EVENT_IOC_RESET, 0);
	/*ioctl(lock_address.fd4, PERF_EVENT_IOC_RESET, 0);
	ioctl(lock_address.fd5, PERF_EVENT_IOC_RESET, 0);*/
	ioctl(lock_address.fd1, PERF_EVENT_IOC_ENABLE, 0);
	ioctl(lock_address.fd2, PERF_EVENT_IOC_ENABLE, 0);
	ioctl(lock_address.fd3, PERF_EVENT_IOC_ENABLE, 0);

        return 0;
}

int out_lock_info_test(){

	long long count1, count2, count3/*, count4, count5*/;
	ioctl(lock_address.fd1, PERF_EVENT_IOC_DISABLE, 0);
	ioctl(lock_address.fd2, PERF_EVENT_IOC_DISABLE, 0);
	ioctl(lock_address.fd3, PERF_EVENT_IOC_DISABLE, 0);
	if (read(lock_address.fd1, &count1, sizeof(long long)) < 0) exit(0);
	//count2 = 0;
	if (read(lock_address.fd2, &count2, sizeof(long long)) < 0) exit(0);
	if (read(lock_address.fd3, &count3, sizeof(long long)) < 0) exit(0);
	/*ignore = read(lock_address.fd4, &count4, sizeof(long long));
	ignore = read(lock_address.fd5, &count5, sizeof(long long));*/
	setlocale(LC_NUMERIC, "");
	fprintf(stderr, "Total branches %'lld, missing branches without kernel %'lld and with kernel %'lld\n", count1, count2, count3);
        for (int i = 0; i < lock_address.count; i++){
                custom_lock *lock = (custom_lock*)(lock_address.lock[i]);
                double a = (lock->__temp_a - lock->__start_a) / (lock->__N);
                double b = lock->__b / (lock->__N);
                double w = lock->__w / (lock->__N);
                double y = b / a;
                double u = w + b;
                double l = w / a;
                double m = u / a;
                double w_cpu = lock->__cpu_w / (lock->__N);
		struct rusage test;
		getrusage(RUSAGE_SELF, &test);
		fprintf(stderr, "Before quanta %'ld, cause of quanta %'ld\n", test.ru_nvcsw, test.ru_nivcsw);
		fprintf(stderr, "Branch-missing in waiting lock: %'lld, branch-missing in critical: %'lld, branch-missing out: %'lld, branch_missing unlock: %'lld\n", (long long)0, (long long)0, (long long)0, (long long)0);
                fprintf(stderr, OUTPUT, (void*)lock, lock->__N, b, a, w, w_cpu, y, u, l, m);
				//fprintf(stderr, "CPU in crit: %f\n", lock->__b_cpu / (lock->__N));
		//fprintf(stderr, "Max time in critical section: %f\n", lock->__b_max);
                for (int j = 0; j < lock->__itid; j++){
			int count = lock->__content_thr[j].__count;
			double w_thr = lock->__content_thr[j].__wait_time / count;
			double b_thr = lock->__content_thr[j].__section_time / count;
                        fprintf(stderr, OUTPUT2, lock->__content_thr[j].__tid, j, count, b_thr, w_thr, lock->__content_thr[j].__max_wait_time);
		}
        }
        free(lock_address.lock);
        return 0;
}

#endif
#endif
