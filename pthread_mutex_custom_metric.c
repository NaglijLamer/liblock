#include "pthread_mutex_custom_metric.h"
//#include <stddef.h> 

int pthread_mutex_lock_metric_c(pthread_mutex_metric_t_c *lock){
	METRIC_BEFORE_LOCK;
	int ign0, ign1;
	__asm __volatile(
	//"int $3\n\t"

	"lock; cmpxchgl %3, %1\n\t"
	"jz 3f\n\t"

	"xorq %%r10, %%r10\n\t"
	"movl $2, %%edx\n\t"
#ifdef MULTIPROC
	"xorl %%esi, %%esi\n\t" //FUTEX_WAIT
#else
	"movl $128, %%esi\n\t" //FUTEX_WAIT_PRIVATE
#endif
	"cmpl %%edx, %%eax\n\t"
	"jne 2f\n\t"

	"1:\tmovl $202, %%eax\n\t" //SYS_futex == 202
	"syscall\n\t"
#ifdef _DEBUG_LIBLOCK
	"cmpq $0, %%rax\n\t"
	"je 2f\n\t"
	"int $3\n\t"
#endif
	"2:\tmovl %%edx, %%eax\n\t"
	"xchgl %%eax, %1\n\t"
	"testl %%eax, %%eax\n\t"
	"jnz 1b\n\t"

	"3:"
	: "=S" (ign0), "=m" (*lock), "=a" (ign1)
	: "0" (1), "m" (*lock), "2" (0), "D" (lock)
	: "cx", "r11", "cc", "memory");

	METRIC_AFTER_LOCK;
	return 0;
}

int pthread_mutex_unlock_metric_c(pthread_mutex_metric_t_c *lock){
	METRIC_BEFORE_UNLOCK;
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

	"3:"
	: "=m" (*lock)
	: "m" (*lock), "D" (lock)
	: "cx", "r11", "cc", "memory");
	return 0;
}

int pthread_mutex_init_metric_c(pthread_mutex_metric_t_c *lock, void *attr){
	METRIC_INIT_LOCK;
	if (attr != NULL && (*(int*)attr) == 1) lock->__lock = 1;
	else lock->__lock = 0;
	lock->__spins = 0;
	return 0;
}
