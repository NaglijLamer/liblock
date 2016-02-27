#ifndef _TICKET_SPINLOCK_H
#define _TICKET_SPINLOCK_H

typedef struct{
	int next;
	int serv;
}ticket_spinlock_t;
//typedef volatile int ticket_spinlock_t;

extern int ticket_spin_init(ticket_spinlock_t *lock, int locked);
extern int ticket_spin_lock(ticket_spinlock_t *lock);
extern int ticket_spin_unlock(ticket_spinlock_t *lock);

#endif
