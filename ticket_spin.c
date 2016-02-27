#include "ticket_spin.h"

int ticket_spin_lock(ticket_spinlock_t *lock){
	int ign;
	__asm __volatile(
	"lock; xaddl %0, %1\n\t"
	"jmp 2f\n\t"
	"1:\tpause\n\t"
	//"1:\t"
	"2:\tcmpl %0, 4%1\n\t"
	"jne 1b\n\t"
	: "=a" (ign), "=m" (*lock)
	: "0" (1), "m" (*lock));
	//: "cc", "memory");
	return 0;
}

int ticket_spin_unlock(ticket_spinlock_t *lock){
	lock->serv++;
	//__asm __volatile(
	//"movl $1, %0\n\t"
	//: "=m" (*lock)
	//: "m" (*lock));
	return 0;
}

int ticket_spin_init(ticket_spinlock_t *lock, int locked){
	lock->serv = 0;
	lock->next = locked > 0? 1 : 0;
	return 0;
}
//int ticket_spin_init(ticket_spinlock_t *lock, int ignore) __attribute__ ((weak, alias("pthread_spin_unlock")));

