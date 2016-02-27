#ifndef _CUSTOM_MUTEX_H
#define _CUSTOM_MUTEX_H

#define SYS_futex 202
#define FUTEX_WAIT 0
#define FUTEX_PRIVATE_FLAG 128
#define FUTEX_WAKE 1
	
typedef volatile int pthread_mutex_t_c;

/*typedef struct{
	int __lock;
} pthread_mutex_t_c;*/

extern int pthread_mutex_init_c(pthread_mutex_t_c *mutex, void *attr);
extern int pthread_mutex_lock_c(pthread_mutex_t_c *mutex);
extern int pthread_mutex_unlock_c(pthread_mutex_t_c *mutex);

#endif
