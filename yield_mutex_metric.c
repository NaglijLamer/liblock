#include "yield_mutex_metric.h"
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sched.h>


int yield_mutex_unlock_metric(pthread_mutex_metric_t_c *lock){
	METRIC_BEFORE_UNLOCK(lock);
	__asm __volatile(

	"lock; decl %0\n\t"
	"jz 3f\n\t"
	
	"movl $0, %0\n\t"
	"movl $1, %%edx\n\t"
#ifdef MULTIPROC
	"mov %%edx, %%esi\n\t" //FUTEX_WAKE
#else
	"movl $129, %%esi\n\t" //FUTEX_WAKE_PRIVATE
#endif
	"movl $202, %%eax\n\t" //SYS_futex == 202
	"syscall\n\t"
	/*"movl $24, %%eax\n\t" //SYS_sched_yield == 24
	"syscall\n\t"*/
	#ifdef _DEBUG_LIBLOCK
	"cmpq $2, %%rax\n\t"
	"je 3f\n\t"
	"int $3\n\t"
#endif

	"3:"
	: "=m" (*lock)
	: "m" (*lock), "D" (lock)
	: "cx", "r11", "cc", "memory");
	//fprintf(stderr, "%d\n", SYS_sched_yield);
	/*struct sched_param sch;
	sched_getparam(0, &sch);
	sch.sched_priority--;
	sched_setparam(0, &sch);
	syscall(SYS_sched_yield);*/
	//usleep(0);
	METRIC_AFTER_UNLOCK(lock);
	return 0;

}

