CC=gcc
CCP=g++
CFLAGSNEW=-Wall -Wextra -std=gnu99 -pthread -lrt -O3 -D_REENTRANT
CFLAGSL=$(CFLAGSNEW) -fpic
CFLAGSTIME=-Wall -Wextra -std=gnu99 -pthread -O3 -D_REENTRANT -D_GLOBAL_TIMER_LOCK
CFLAGSM=$(CFLAGSNEW) -D_METRIC
CFLAGSMS=$(CFLAGSM) -D_SHORT_METRIC
CFLAGSOLD=-Wall -Wextra -std=gnu99 -pedantic -pthread -O3 -D_REENTRANT
CFLAGSNEW2=-Wall -Wextra -std=gnu99 -pedantic -pthread -D_REENTRANT
LIBRARY=-llock -lrt
CLEAR_ASM=-Wall -Wextra -std=gnu99 -pedantic -pthread -S
TEST_NAME=test_m.c
#DIR=/home/naglijlamer/liv
DIR=$(shell pwd)
PREPR=$(CFLAGSNEW) -E
ASM=$(CFLAGSNEW) -S 
DEBUG=$(CFLAGSNEW) -g

.DEFAULT_GOAL := all

#library compilation 
library: library_compile library_clean
	@true
libmutex: pthread_mutex_custom.c pthread_mutex_custom.h
	$(CC) -c $(CFLAGSL) pthread_mutex_custom.c -o libmutex.o
libspin: pthread_spin_custom.c pthread_spin_custom.h
	$(CC) -c $(CFLAGSL) pthread_spin_custom.c -o libspin.o
libticket: ticket_spin.c ticket_spin.h
	$(CC) -c $(CFLAGSL) ticket_spin.c -o libticket.o
libMCS: MCS_spin.h MCS_spin.c
	$(CC) -c $(CFLAGSL) MCS_spin.c -o libMCS.o
libadaptive: adaptive_mutex.c adaptive_mutex.h
	$(CC) -c $(CFLAGSL) adaptive_mutex.c -o libadaptive.o
libyield: yield_mutex.c yield_mutex.h
	$(CC) -c $(CFLAGSL) yield_mutex.c -o libyield.o
libspinmetric: metrics.h pthread_spin_custom_metric.c pthread_spin_custom_metric.h
	$(CC) -c $(CFLAGSL) pthread_spin_custom_metric.c -o libspinmetric.o
libmutexmetric: metrics.h pthread_mutex_custom_metric.c pthread_mutex_custom_metric.h
	$(CC) -c $(CFLAGSL) pthread_mutex_custom_metric.c -o libmutexmetric.o
libposixmutexmetric: metrics.h pthread_mutex_metric.h pthread_mutex_metric.c
	$(CC) -c $(CFLAGSL) pthread_mutex_metric.c -o libposixmutexmetric.o
libticketmetric: metrics.h ticket_spin_metric.c ticket_spin_metric.h
	$(CC) -c $(CFLAGSL) ticket_spin_metric.c -o libticketmetric.o
libadaptivemetric: metrics.h adaptive_mutex_metric.c adaptive_mutex_metric.h
	$(CC) -c $(CFLAGSL) adaptive_mutex_metric.c -o libadaptivemetric.o
libyieldmetric: metrics.h yield_mutex_metric.c yield_mutex_metric.h
	$(CC) -c $(CFLAGSL) yield_mutex_metric.c -o libyieldmetric.o
libposixspinmetric: metrics.h pthread_spin_metric.h pthread_spin_metric.c
	$(CC) -c $(CFLAGSL) pthread_spin_metric.c -o libposixspinmetric.o
libMCSmetric: metrics.h MCS_spin_metric.h MCS_spin_metric.c
	$(CC) -c $(CFLAGSL) MCS_spin_metric.c -o libMCSmetric.o
#depricated
libmetric: metrics.h metric_function.h metric_function.c
	$(CC) -c $(CFLAGSL) metric_function.c -o libmetric.o
#depricated
libmetric2: metrics.h global_metric.h global_metric.c
	$(CC) -c $(CFLAGSL) global_metric.c -o libmetric2.o
library_compile: libmutex libspin libticket libMCS libadaptive libspinmetric libmutexmetric libposixmutexmetric libticketmetric libadaptivemetric libposixspinmetric libMCSmetric libmetric libmetric2 libyield libyieldmetric
	$(CC) -shared libmutex.o libspin.o libticket.o libMCS.o libadaptive.o libmetric.o libmetric2.o libspinmetric.o libmutexmetric.o libposixmutexmetric.o libticketmetric.o libadaptivemetric.o libposixspinmetric.o libMCSmetric.o libyield.o libyieldmetric.o -o liblock.so
library_clean:
	rm libmutex.o libspin.o libticket.o libMCS.o libadaptive.o libspinmetric.o libmetric2.o libmetric.o libmutexmetric.o libposixmutexmetric.o libticketmetric.o libadaptivemetric.o libposixspinmetric.o libMCSmetric.o libyield.o libyieldmetric.o

#probably some stuff to generate output file names
#compile test files to get test_LOCK_time apps 
test_spin_time: $(TEST_NAME) library
	$(CC) $(CFLAGSTIME) -D_SPINCLASSIC -L$(DIR) $(TEST_NAME) -o test_spin_time $(LIBRARY)
test_mutex_time: $(TEST_NAME) library
	$(CC) $(CFLAGSTIME) -D_MUTEX -L$(DIR) $(TEST_NAME) -o test_mutex_time $(LIBRARY)
test_posixmutex_time: $(TEST_NAME) library
	$(CC) $(CFLAGSTIME) -D_POSIXMUTEX -L$(DIR) $(TEST_NAME) -o test_posixmutex_time $(LIBRARY)
test_posixspin_time: $(TEST_NAME) library
	$(CC) $(CFLAGSTIME) -D_POSIXSPIN -L$(DIR) $(TEST_NAME) -o test_posixspin_time $(LIBRARY)
test_ticket_time: $(TEST_NAME) library
	$(CC) $(CFLAGSTIME) -D_TICKETSPIN -L$(DIR) $(TEST_NAME) -o test_ticket_time $(LIBRARY)
test_MCS_time: $(TEST_NAME) library
	$(CC) $(CFLAGSTIME) -D_MCS_SPIN -L$(DIR) $(TEST_NAME) -o test_MCS_time $(LIBRARY)
test_adaptive_time: $(TEST_NAME) library
	$(CC) $(CFLAGSTIME) -D_ADAPTIVE -L$(DIR) $(TEST_NAME) -o test_adaptive_time $(LIBRARY)
test_yield_time: $(TEST_NAME) library
	$(CC) $(CFLAGSTIME) -D_YIELD -L$(DIR) $(TEST_NAME) -o test_yield_time $(LIBRARY)
test_time: test_spin_time test_ticket_time test_mutex_time test_posixmutex_time test_posixspin_time test_MCS_time test_adaptive_time test_yield_time
	@true

#compile test files to get test_LOCK normal apps
test_spin: $(TEST_NAME) library
	$(CC) $(CFLAGSNEW) -D_SPINCLASSIC -L$(DIR) $(TEST_NAME) -o test_spin $(LIBRARY)
test_ticket: $(TEST_NAME) library
	$(CC) $(CFLAGSNEW) -D_TICKETSPIN -L$(DIR) $(TEST_NAME) -o test_ticket $(LIBRARY) 
test_mutex: $(TEST_NAME) library
	$(CC) $(CFLAGSNEW) -D_MUTEX -L$(DIR) $(TEST_NAME) -o test_mutex $(LIBRARY)
test_posix_mutex: $(TEST_NAME) library
	$(CC) $(CFLAGSNEW) -D_POSIXMUTEX -L$(DIR) $(TEST_NAME) -o test_posixmutex $(LIBRARY)
test_posix_spin: $(TEST_NAME) library
	$(CC) $(CFLAGSNEW) -D_POSIXSPIN -L$(DIR) $(TEST_NAME) -o test_posixspin $(LIBRARY)
test_MCS: $(TEST_NAME) library
	$(CC) $(CFLAGSNEW) -D_MCS_SPIN -L$(DIR) $(TEST_NAME) -o test_MCS $(LIBRARY)
test_adaptive: $(TEST_NAME) library
	$(CC) $(CFLAGSNEW) -D_ADAPTIVE -L$(DIR) $(TEST_NAME) -o test_adaptive $(LIBRARY)
test_yield: $(TEST_NAME) library
	$(CC) $(CFLAGSNEW) -D_YIELD -L$(DIR) $(TEST_NAME) -o test_yield $(LIBRARY)
test: test_spin test_ticket test_mutex test_posix_mutex test_posix_spin test_MCS test_adaptive test_yield
	@true

#compile test files to get metric_LOCK apps
test_spin_metric: $(TEST_NAME) library
	$(CC) $(CFLAGSM) -D_SPINCLASSIC -L$(DIR) $(TEST_NAME) -o metric_spin $(LIBRARY)
test_ticket_metric: $(TEST_NAME) library
	$(CC) $(CFLAGSM) -D_TICKETSPIN -L$(DIR) $(TEST_NAME) -o metric_ticket $(LIBRARY)
test_mutex_metric: $(TEST_NAME) library
	$(CC) $(CFLAGSM) -D_MUTEX -L$(DIR) $(TEST_NAME) -o metric_mutex $(LIBRARY)
test_posix_mutex_metric: $(TEST_NAME) library
	$(CC) $(CFLAGSM) -D_POSIXMUTEX -L$(DIR) $(TEST_NAME) -o metric_posixmutex $(LIBRARY)
test_posix_spin_metric: $(TEST_NAME) library
	$(CC) $(CFLAGSM) -D_POSIXSPIN -L$(DIR) $(TEST_NAME) -o metric_posixspin $(LIBRARY)
test_MCS_metric: $(TEST_NAME) library
	$(CC) $(CFLAGSM) -D_MCS_SPIN -L$(DIR) $(TEST_NAME) -o metric_MCS $(LIBRARY)
test_adaptive_metric: $(TEST_NAME) library
	$(CC) $(CFLAGSM) -D_ADAPTIVE -L$(DIR) $(TEST_NAME) -o metric_adaptive $(LIBRARY)
test_yield_metric: $(TEST_NAME) library
	$(CC) $(CFLAGSM) -D_YIELD -L$(DIR) $(TEST_NAME) -o metric_yield $(LIBRARY)
test_metric: test_spin_metric test_ticket_metric test_mutex_metric test_posix_mutex_metric test_posix_spin_metric test_MCS_metric test_adaptive_metric test_yield_metric
	@true

#short output for parsing scripts
test_spin_metric_s: $(TEST_NAME) library
	$(CC) $(CFLAGSMS) -D_SPINCLASSIC -L$(DIR) $(TEST_NAME) -o smetric_spin $(LIBRARY)
test_ticket_metric_s: $(TEST_NAME) library
	$(CC) $(CFLAGSMS) -D_TICKETSPIN -L$(DIR) $(TEST_NAME) -o smetric_ticket $(LIBRARY)
test_mutex_metric_s: $(TEST_NAME) library
	$(CC) $(CFLAGSMS) -D_MUTEX -L$(DIR) $(TEST_NAME) -o smetric_mutex $(LIBRARY)
test_posix_mutex_metric_s: $(TEST_NAME) library
	$(CC) $(CFLAGSMS) -D_POSIXMUTEX -L$(DIR) $(TEST_NAME) -o smetric_posixmutex $(LIBRARY)
test_posix_spin_metric_s: $(TEST_NAME) library
	$(CC) $(CFLAGSMS) -D_POSIXSPIN -L$(DIR) $(TEST_NAME) -o smetric_posixspin $(LIBRARY)
test_MCS_metric_s: $(TEST_NAME) library
	$(CC) $(CFLAGSMS) -D_MCS_SPIN -L$(DIR) $(TEST_NAME) -o smetric_MCS $(LIBRARY)
test_adaptive_metric_s: $(TEST_NAME) library
	$(CC) $(CFLAGSMS) -D_ADAPTIVE -L$(DIR) $(TEST_NAME) -o smetric_adaptive $(LIBRARY)
test_yield_metric_s: $(TEST_NAME) library
	$(CC) $(CFLAGSMS) -D_YIELD -L$(DIR) $(TEST_NAME) -o smetric_yield $(LIBRARY)
test_metric_s: test_spin_metric_s test_ticket_metric_s test_mutex_metric_s test_posix_mutex_metric_s test_posix_spin_metric_s test_MCS_metric_s test_adaptive_metric_s test_yield_metric_s
	@true

#everything
all: test test_time test_metric test_metric_s
	@true

#test stuff
test2: library test.c
	$(CC) $(CFLAGSNEW) -D_SPINCLASSIC -D_METRIC -L$(DIR) test.c -o metric_spin $(LIBRARY)
test_3: library test.c
	$(CC) $(CFLAGSNEW) -D_SPINCLASSIC -D_METRIC -E -L$(DIR) test.c -o metric_spin $(LIBRARY)
test_4: library test.c
	$(CC) $(CFLAGSNEW2) -D_SPINCLASSIC -D_METRIC -L$(DIR) test.c -o metric_spin $(LIBRARY)
test5: library test.c
	$(CC) $(CFLAGSNEW) -D_MUTEX -D_METRIC -L$(DIR) test.c -o metric_mutex $(LIBRARY)
test6: library test.c
	$(CC) $(CFLAGSNEW) -D_POSIXMUTEX -D_METRIC -L$(DIR) test.c -o metric_posixmutex $(LIBRARY)
