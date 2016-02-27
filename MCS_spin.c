/*Test implementation! 
It uses pthread_key_t to get thread specific element of spinlock*/

#include "MCS_spin.h"
#include <malloc.h>
//#include <stdio.h>

int MCS_spin_lock(MCS_lock_t *lock){
	MCS_node *I, *pred;
	long long ign0;
	//MCS_node *I;
	//int ign0, ign1;
	if ((I = (MCS_node*)pthread_getspecific(lock->key)) == NULL){
		I = (MCS_node*)malloc(sizeof(MCS_node));
		pthread_setspecific(lock->key, (void*)I);
		//printf("Value of setspec is %d", m);
		//fflush(stdout);
		I->locked = 0;
	}
	//printf("%sSizeoflock %d, size ofnode %d and size of point %d, sizeof key %d, pointer to I %p and key value%d aaand pointer tp L %p pointers to lock %p and to L %p\n", "Try to got lock\n", sizeof(MCS_lock_t), sizeof(MCS_node), sizeof(MCS_node*), sizeof(pthread_key_t), I, lock->key, lock->L, lock, &(lock->L));
	//fflush(stdout);
	__asm __volatile(
		"movq %q4, (%1)\n\t"
		"movq %1, %3\n\t"
		"lock; xchgq %3, 8(%0)\n\t"
		"testq %3, %3\n\t"
		"jz 3f\n\t"
		"movq $1, 8(%1)\n\t"
		"movq %1, (%3)\n\t"
		"jmp 2f\n\t"
		"1:\tpause\n\t"
		"2:\tcmpl %4, 8(%1)\n\t"
		"jnz 1b\n\t"
		"3:"
	: "=D" (lock), "=S" (I), "=d" (ign0), "=c" (pred)
	: "2" (0), "S" (I), "D" (lock));
	/*printf("%s", "Got lock\n");
	fflush(stdout);*/
	return 0;
}

int MCS_spin_unlock(MCS_lock_t *lock){
	long long ign0;
	MCS_node *I = (MCS_node*)pthread_getspecific(lock->key);
	/*printf("%s", "Try to realize lock\n");
	fflush(stdout);*/
	__asm __volatile(
		"cmpq %q3, (%1)\n\t"
		"jne 1f\n\t"
		"movq %1, %%rax\n\t"
		"lock; cmpxchgq %q3, 8(%0)\n\t"
		"jz 2f\n\t"
		"3:\tpause\n\t"
		"cmpq %q3, (%1)\n\t"
		//"jne 3b\n\t"
		"je 3b\n\t"
		"1:\tmovq (%1), %%rax\n\t"
		"movq %q3, 8(%%rax)\n\t"
		"2:"
	: "=D" (lock), "=S" (I), "=d" (ign0) 
	: "2" (0), "S" (I), "D" (lock)
	: "rax");
	/*printf("%s", "realize lock\n");
	fflush(stdout);*/
	return 0;
}

int MCS_spin_init(MCS_lock_t *lock, int locked){
	//Second argument must be not NULL, but pointer to destructor.
	pthread_key_create((pthread_key_t*)lock, NULL);
	lock->L = NULL;
	return 0;
}

