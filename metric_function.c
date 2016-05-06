#include <stdio.h>
#include <malloc.h>
#include "metric_function.h"
#include "metrics.h"

#ifdef _SHORT_METRIC
#define OUTPUT "%p\n%ld\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n"
#define OUTPUT "%d %d %d\n"
#else
#define OUTPUT "Lock address: %p\nAmount of threads wanting to get lock: %ld\nAverage in critical section: %f\nAverage between two threads on lock: %f\nAverage waiting time: %f\nAverage waiting CPU time: %f\nLoading: %f\nAverage time in section+waitng: %f\nAverage queue: %f\nAverage amount of threads in section+waiting: %f\nPer thread locks:\n"
#define OUTPUT2 "Linux TID: %d. Thread internal id on this lock: %d. Amount of attempt to get lock: %d\n"
#endif

int in_lock_info_test(){
        lock_address.count = 0;
        lock_address.size = 10;
        lock_address.lock = (void**)malloc(sizeof(void*) * lock_address.size);
        return 0;
}

int out_lock_info_test(){
        for (int i = 0; i < lock_address.count; i++){
                custom_lock *lock = (custom_lock*)(lock_address.lock[i]);
                //double a = lock->__a / (lock->__Na);
		double a = (lock->__temp_a - lock->__start_a) / (lock->__N);
                double b = lock->__b / (lock->__N);
                double w = lock->__w / (lock->__N);
                double y = b / a;
                double u = w + b;
                double l = w / a;
                double m = u / a;
		double w_cpu = lock->__cpu_w / (lock->__N);
                //fprintf(stderr, "Lock address: %p\nAmount of threads wanting to get lock: %ld\nAverage in critical section: %f\nAverage between two threads on lock: %f\nAverage waiting time: %f\nLoading: %f\nAverage time in section+waitng: %f\nAverage queue: %f\nAverage amount of threads in section+waiting: %f\n", (void *)lock, lock->__N, b, a, w, y, u, l, m);
		fprintf(stderr, OUTPUT, (void*)lock, lock->__N, b, a, w, w_cpu, y, u, l, m);
		for (int j = 0; j < lock->__itid; j++)
			fprintf(stderr, OUTPUT2, lock->__content_thr[j].__tid, j, lock->__content_thr[j].__count);
        }
	free(lock_address.lock);
        return 0;
}

