#include "yield_mutex.h"

int yield_mutex_unlock(pthread_mutex_t_c *mutex){
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
	"movl $24, %%eax\n\t" //SYS_sched_yield == 24
	"syscall\n\t"

	"3:"
	: "=m" (*mutex)
	: "m" (*mutex)
	: "cx", "r11", "cc", "memory");
	return 0;

}

