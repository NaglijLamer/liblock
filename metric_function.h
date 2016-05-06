#ifndef _METRIC_FUNCTION_H
#define _METRIC_FUNCTION_H

int in_lock_info_test() __attribute__((constructor(101)));
int out_lock_info_test() __attribute__((destructor(101)));

#endif
