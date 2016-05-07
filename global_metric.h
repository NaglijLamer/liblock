#if defined(_METRIC) || defined(_GLOBAL_TIMER_LOCK)
#ifndef _GLOBAL_METRIC_H
#define _GLOBAL_METRIC_H

#include <time.h>
#include "common_defs.h"
#include <stdio.h>

#ifdef _SHORT_METRIC
#define OUTPUTGL "%f\n"
#else
#define OUTPUTGL "Program execution time: %f\n"
#endif

struct timespec __start_time;

int start_global_timer() __attribute__((constructor(102)));
int finish_global_timer() __attribute__((destructor(102)));

int start_global_timer(){
        clock_gettime(CLOCK_MONOTONIC, &__start_time);
        return 0;
}

int finish_global_timer(){
        struct timespec finish_time;
        clock_gettime(CLOCK_MONOTONIC, &finish_time);
        double time = TIMESPEC_TO_DOUBLE(finish_time) - TIMESPEC_TO_DOUBLE(__start_time);
        fprintf(stderr, OUTPUTGL, time);
        return 0;
}

#endif
#endif
