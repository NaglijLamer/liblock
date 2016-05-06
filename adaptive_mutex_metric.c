#include "adaptive_mutex_metric.h"

#define TRYLOCK(futex) \
	({int ret;	\
	__asm __volatile (	\
		"lock; cmpxchgl %2, %1\n\t"	\
		: "=a" (ret), "=m" (futex)	\
		: "r" (1), "m" (futex), "0" (0)	\
		: "memory");	\
	ret;})

int adaptive_mutex_lock_metric(pthread_mutex_metric_t_c *lock){
	METRIC_BEFORE_LOCK;
	int ign0, ign1;
	if (TRYLOCK(lock->__lock) != 0){
		int cnt = 0;
		int spins = lock->__spins * 2 + 10;
		int max_cnt = spins > 100 ? 100 : spins;
		do{
			if (cnt++ >= max_cnt){
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
        			: "=S" (ign0), "=m" (*lock), "=a" (ign1)
        			: "0" (1), "m" (*lock), "2" (0), "D" (lock)
        			: "cx", "dx", "r11", "cc", "memory");
				break;
			}
		__asm__ ("pause");
		} while (TRYLOCK(lock->__lock) != 0);
		lock->__spins += (cnt - lock->__spins) / 8;
	}
	METRIC_AFTER_LOCK;
	return 0;
}

