CC=gcc
CCP=g++
CFLAGSNEW=-Wall -Wextra -std=gnu99 -pedantic -pthread -lrt -O2 -D_REENTRANT
CLEAR_ASM=-Wall -Wextra -std=gnu99 -pedantic -pthread -lrt -S
PREPR=$(CFLAGSNEW) -E
ASM=$(CFLAGSNEW) -S 
DEBUG=$(CFLAGSNEW) -g

library: custom_lock.h pthread_mutex_custom.c pthread_mutex_custom.h pthread_spin_custom.c pthread_spin_custom.h ticket_spin.c ticket_spin.h MCS_spin.h MCS_spin.c
	$(CC) -c $(CFLAGSNEW) -fpic pthread_mutex_custom.c -o libmutex.o
	$(CC) -c $(CFLAGSNEW) -fpic pthread_spin_custom.c -o libspin.o
	$(CC) -c $(CFLAGSNEW) -fpic ticket_spin.c -o libticket.o
	$(CC) -c $(CFLAGSNEW) -fpic MCS_spin.c -o libMCS.o
	$(CC) -shared libmutex.o libspin.o libticket.o libMCS.o -o liblock.so
	rm libmutex.o
	rm libspin.o
	rm libticket.o
	rm libMCS.o
test_spin: test.c library
	$(CC) $(CFLAGSNEW) -D_SPINCLASSIC -L/home/naglijlamer/liv test.c -o test_spin -llock
test_ticket: test.c library
	$(CC) $(CFLAGSNEW) -D_TICKETSPIN -L/home/naglijlamer/liv test.c -o test_ticket -llock
test_mutex: test.c library
	$(CC) $(CFLAGSNEW) -D_MUTEX -L/home/naglijlamer/liv test.c -o test_mutex -llock
test_posix: test.c library
	$(CC) $(CFLAGSNEW) -L/home/naglijlamer/liv test.c -o test_posix -llock
test_MCS: test.c library
	$(CC) $(CFLAGSNEW) -D_MCS_SPIN -L/home/naglijlamer/liv test.c -o test_MCS -llock
test: test_spin test_ticket test_mutex test_posix test_MCS
	@true
