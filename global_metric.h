#if defined(_METRIC) || defined(_GLOBAL_TIMER_LOCK)
#ifndef _GLOBAL_METRIC_H
#define _GLOBAL_METRIC_H

#include <time.h>
/*
//struct timespec __start_time;*/

#include "common_defs.h"
#include <stdio.h>
#include <time.h>

#ifdef __SHORT_METRIC
#define OUTPUTGL "%f\n"
#else
#define OUTPUTGL "Program execution time: %f\n"
#endif

//struct timespec __start_time, lol;
struct timespec __start_time;

int start_global_timer() __attribute__((constructor(102)));
int finish_global_timer() __attribute__((destructor(102)));

int start_global_timer(){
        //fprintf(stderr, "%s\n", "LOL");
        clock_gettime(CLOCK_MONOTONIC, &__start_time);
        //clock_gettime(CLOCK_THREAD_CPUTIME_ID, &lol);
        return 0;
}

int finish_global_timer(){
        //struct timespec finish_time, finish2;
        struct timespec finish_time;
        clock_gettime(CLOCK_MONOTONIC, &finish_time);
        //clock_gettime(CLOCK_THREAD_CPUTIME_ID, &finish2);
        double time = TIMESPEC_TO_DOUBLE(finish_time) - TIMESPEC_TO_DOUBLE(__start_time);
        //double time2 = TIMESPEC_TO_DOUBLE(finish2) - TIMESPEC_TO_DOUBLE(lol);
        //fprintf(stderr, "Program execution time: %f\n", time);
        fprintf(stderr, OUTPUTGL, time);
        //fprintf(stderr, "CPU TIME %f\n", time2);
        return 0;
}


#endif
#endif
