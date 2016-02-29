#include "adaptive_mutex.h"

#define TRYLOCK(futex) \
	({int ret;	\
	__asm __volatile (	\
		"lock; cmpxchgl %2, %1\n\t"	\
		: "=a" (ret), "=m" (futex)	\
		: "r" (1), "m" (futex), "0" (0)	\
		: "memory");	\
	ret;})

int adaptive_mutex_lock(pthread_mutex_t_c *mutex){
	int ign0, ign1;
	if (TRYLOCK(mutex->__lock) != 0){
		int cnt = 0;
		int spins = mutex->__spins * 2 + 10;
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
        			: "=S" (ign0), "=m" (*mutex), "=a" (ign1)
        			: "0" (1), "m" (*mutex), "2" (0)
        			: "cx", "dx", "r10", "cc", "memory");
				break;
			}
		__asm__ ("pause");
		} while (TRYLOCK(mutex->__lock) != 0);
		mutex->__spins += (cnt - mutex->__spins) / 8;
	}
	return 0;
}

