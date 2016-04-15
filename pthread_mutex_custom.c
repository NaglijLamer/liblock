#include "pthread_mutex_custom.h"
#include <stddef.h>

int pthread_mutex_lock_c(pthread_mutex_t_c *mutex){
	int ign0, ign1;
	__asm __volatile(

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
	"2:\tmovl %%edx, %%eax\n\t"
	"xchgl %%eax, %1\n\t"
	"testl %%eax, %%eax\n\t"
	"jnz 1b\n\t"

	"3:"
	: "=S" (ign0), "=m" (*mutex), "=a" (ign1)
	: "0" (1), "m" (*mutex), "2" (0)
	: "cx", "r10", "cc", "memory");

	return 0;
}

int pthread_mutex_unlock_c(pthread_mutex_t_c *mutex){
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
	: "=m" (*mutex)
	: "m" (*mutex)
	: "cc", "memory");
	return 0;
}

int pthread_mutex_init_c(pthread_mutex_t_c *mutex, void *attr){
	if (attr != NULL && (*(int*)attr) == 1) mutex->__lock = 1;
	else mutex->__lock = 0;
	mutex->__spins = 0;
	return 0;
}
