#include "pthread_spin_custom.h"

int pthread_spin_lock_c(pthread_spinlock_t_c *lock){
	__asm __volatile(
	"1:\tlock; decl %0\n\t"
	"jne 2f\n\t"
	".subsection 1\n\t"
	".align 16\n"
	"2:\tpause\n\t"
	//"2:"
	//"2:\trep; nop\n\t"
	"cmpl $0, %0\n\t"
	"jg 1b\n\t"
	"jmp 2b\n\t"
	".previous"
	: "=m" (*lock)
	: "m" (*lock));

	return 0;
}

int pthread_spin_unlock_c(pthread_spinlock_t_c *lock){
	__asm __volatile(
	"movl $1, %0\n\t"
	: "=m" (*lock)
	: "m" (*lock));
	return 0;
}

int pthread_spin_init_c(pthread_spinlock_t_c *lock, int ignore) __attribute__ ((weak, alias("pthread_spin_unlock_c")));
/*int pthread_spin_init_c(pthread_spinlock_t_c *lock, int ignore){
        __asm __volatile(
        "movl $1, %0\n\t"
        : "=m" (*lock)
        : "m" (*lock));
        return 0;
}*/


