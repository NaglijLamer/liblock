CC=gcc
CCP=g++
CFLAGSNEW=-Wall -Wextra -std=gnu99 -pedantic -pthread -lrt -O3 -D_REENTRANT
CLEAR_ASM=-Wall -Wextra -std=gnu99 -pedantic -pthread -lrt -S
DIR=/home/naglijlamer/vkr/liblock
PREPR=$(CFLAGSNEW) -E
ASM=$(CFLAGSNEW) -S 
DEBUG=$(CFLAGSNEW) -g

library: custom_lock.h pthread_mutex_custom.c pthread_mutex_custom.h pthread_spin_custom.c pthread_spin_custom.h ticket_spin.c ticket_spin.h MCS_spin.h MCS_spin.c adaptive_mutex.c adaptive_mutex.h
	$(CC) -c $(CFLAGSNEW) -fpic pthread_mutex_custom.c -o libmutex.o
	$(CC) -c $(CFLAGSNEW) -fpic pthread_spin_custom.c -o libspin.o
	$(CC) -c $(CFLAGSNEW) -fpic ticket_spin.c -o libticket.o
	$(CC) -c $(CFLAGSNEW) -fpic MCS_spin.c -o libMCS.o
	$(CC) -c $(CFLAGSNEW) -fpic adaptive_mutex.c -o libadaptive.o
	$(CC) -shared libmutex.o libspin.o libticket.o libMCS.o libadaptive.o -o liblock.so
	rm libmutex.o libspin.o libticket.o libMCS.o libadaptive.o
test_spin: test.c library
	$(CC) $(CFLAGSNEW) -D_SPINCLASSIC -L$(DIR) test.c -o test_spin -llock
test_ticket: test.c library
	$(CC) $(CFLAGSNEW) -D_TICKETSPIN -L$(DIR) test.c -o test_ticket -llock
test_mutex: test.c library
	$(CC) $(CFLAGSNEW) -D_MUTEX -L$(DIR) test.c -o test_mutex -llock
test_posix_mutex: test.c library
	$(CC) $(CFLAGSNEW) -D_POSIXMUTEX -L$(DIR) test.c -o test_posixmutex -llock
test_posix_spin: test.c library
	$(CC) $(CFLAGSNEW) -D_POSIXSPIN -L$(DIR) test.c -o test_posixspin -llock
test_MCS: test.c library
	$(CC) $(CFLAGSNEW) -D_MCS_SPIN -L$(DIR) test.c -o test_MCS -llock
test_adaptive: test.c library
	 $(CC) $(CFLAGSNEW) -D_ADAPTIVE -L$(DIR) test.c -o test_adaptive -llock
test: test_spin test_ticket test_mutex test_posix_mutex test_posix_spin test_MCS test_adaptive
	@true
