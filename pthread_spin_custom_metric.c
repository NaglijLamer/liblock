#include "pthread_spin_custom_metric.h"

int pthread_spin_lock_metric_c(pthread_spinlock_metric_t_c *lock){
	//double __temp_w, __temp_cpu_w;
	//struct timespec __temp_time, __temp_time_cpu;
	METRIC_BEFORE_LOCK(lock);
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
	
	METRIC_AFTER_LOCK(lock);
	return 0;
}

int pthread_spin_unlock_metric_c(pthread_spinlock_metric_t_c *lock){
	METRIC_BEFORE_UNLOCK(lock);
	__asm __volatile(
	"movl $1, %0\n\t"
	: "=m" (*lock)
	: "m" (*lock));
	METRIC_AFTER_UNLOCK(lock);
	return 0;
}

/*int pthread_spin_init_metric_c(pthread_spinlock_metric_t_c *lock, int ignore) __attribute__ ((weak, alias("pthread_spin_unlock_metric_c")));*/
int pthread_spin_init_metric_c(pthread_spinlock_metric_t_c *lock, __attribute__ ((unused)) int ignore){
	METRIC_INIT_LOCK(lock);
        __asm __volatile(
        "movl $1, %0\n\t"
        : "=m" (*lock)
        : "m" (*lock));
        return 0;
}


