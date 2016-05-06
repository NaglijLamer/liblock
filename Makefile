CC=gcc
CCP=g++
CFLAGSNEW=-Wall -Wextra -std=gnu99 -pthread -lrt -O3 -D_REENTRANT
CFLAGSTIME=-Wall -Wextra -std=gnu99 -pthread -O3 -D_REENTRANT -D_GLOBAL_TIMER_LOCK
CFLAGSOLD=-Wall -Wextra -std=gnu99 -pedantic -pthread -O3 -D_REENTRANT
CFLAGSNEW2=-Wall -Wextra -std=gnu99 -pedantic -pthread -D_REENTRANT
LIBRARY=-llock -lrt
CLEAR_ASM=-Wall -Wextra -std=gnu99 -pedantic -pthread -S
//DIR=/home/naglijlamer/vkr/liblock
DIR=/home/naglijlamer/liv
PREPR=$(CFLAGSNEW) -E
ASM=$(CFLAGSNEW) -S 
DEBUG=$(CFLAGSNEW) -g

#library compilation => create many rules and place it as dependencies?
library: custom_lock.h pthread_mutex_custom.c pthread_mutex_custom.h pthread_spin_custom.c pthread_spin_custom.h ticket_spin.c ticket_spin.h MCS_spin.h MCS_spin.c adaptive_mutex.c adaptive_mutex.h custom_lock_normal.h custom_lock_metric.h metrics.h pthread_spin_custom_metric.c pthread_spin_custom_metric.h metric_function.h metric_function.c global_metric.h global_metric.c pthread_mutex_metric.h pthread_mutex_metric.c ticket_spin_metric.c ticket_spin_metric.h adaptive_mutex_metric.c adaptive_mutex_metric.h pthread_spin_metric.h pthread_spin_metric.c MCS_spin_metric.h MCS_spin_metric.c
	$(CC) -c $(CFLAGSNEW) -fpic pthread_mutex_custom.c -o libmutex.o
	$(CC) -c $(CFLAGSNEW) -fpic pthread_spin_custom.c -o libspin.o
	$(CC) -c $(CFLAGSNEW) -fpic ticket_spin.c -o libticket.o
	$(CC) -c $(CFLAGSNEW) -fpic MCS_spin.c -o libMCS.o
	$(CC) -c $(CFLAGSNEW) -fpic adaptive_mutex.c -o libadaptive.o
	$(CC) -c $(CFLAGSNEW) -fpic pthread_spin_custom_metric.c -o libspinmetric.o
	$(CC) -c $(CFLAGSNEW) -fpic pthread_mutex_custom_metric.c -o libmutexmetric.o
	$(CC) -c $(CFLAGSNEW) -fpic pthread_mutex_metric.c -o libposixmutexmetric.o
	$(CC) -c $(CFLAGSNEW) -fpic ticket_spin_metric.c -o libticketmetric.o
	$(CC) -c $(CFLAGSNEW) -fpic adaptive_mutex_metric.c -o libadaptivemetric.o
	$(CC) -c $(CFLAGSNEW) -fpic pthread_spin_metric.c -o libposixspinmetric.o
	$(CC) -c $(CFLAGSNEW) -fpic MCS_spin_metric.c -o libMCSmetric.o
	$(CC) -c $(CFLAGSNEW) -fpic metric_function.c -o libmetric.o
	$(CC) -c $(CFLAGSNEW) -fpic global_metric.c -o libmetric2.o
	$(CC) -shared libmutex.o libspin.o libticket.o libMCS.o libadaptive.o libmetric.o libmetric2.o libspinmetric.o libmutexmetric.o libposixmutexmetric.o libticketmetric.o libadaptivemetric.o libposixspinmetric.o libMCSmetric.o -o liblock.so
	rm libmutex.o libspin.o libticket.o libMCS.o libadaptive.o libspinmetric.o libmetric2.o libmetric.o libmutexmetric.o libposixmutexmetric.o libticketmetric.o libadaptivemetric.o libposixspinmetric.o libMCSmetric.o

#probably some staff to generate output file names
#compile test files to get test_LOCK_time apps 
test_spin_time: test.c library
	$(CC) $(CFLAGSTIME) -D_SPINCLASSIC -L$(DIR) test.c -o test_spin_time $(LIBRARY)
test_mutex_time: test.c library
	$(CC) $(CFLAGSTIME) -D_MUTEX -L$(DIR) test.c -o test_mutex_time $(LIBRARY)
test_posixmutex_time: test.c library
	$(CC) $(CFLAGSTIME) -D_POSIXMUTEX -L$(DIR) test.c -o test_posixmutex_time $(LIBRARY)
test_posixspin_time: test.c library
	$(CC) $(CFLAGSTIME) -D_POSIXSPIN -L$(DIR) test.c -o test_posixspin_time $(LIBRARY)
test_ticket_time: test.c library
	$(CC) $(CFLAGSTIME) -D_TICKETSPIN -L$(DIR) test.c -o test_ticket_time $(LIBRARY)
test_MCS_time: test.c library
	$(CC) $(CFLAGSTIME) -D_MCS_SPIN -L$(DIR) test.c -o test_MCS_time $(LIBRARY)
test_adaptive_time: test.c library
	$(CC) $(CFLAGSTIME) -D_ADAPTIVE -L$(DIR) test.c -o test_adaptive_time $(LIBRARY)
test_time: test_spin_time test_ticket_time test_mutex_time test_posixmutex_time test_posixspin_time test_MCS_time test_adaptive_time
	@true

#compile test files to get test_LOCK normal apps
test_spin: test.c library
	$(CC) $(CFLAGSNEW) -D_SPINCLASSIC -L$(DIR) test.c -o test_spin $(LIBRARY)
test_ticket: test.c library
	$(CC) $(CFLAGSNEW) -D_TICKETSPIN -L$(DIR) test.c -o test_ticket $(LIBRARY) 
test_mutex: test.c library
	$(CC) $(CFLAGSNEW) -D_MUTEX -L$(DIR) test.c -o test_mutex $(LIBRARY)
test_posix_mutex: test.c library
	$(CC) $(CFLAGSNEW) -D_POSIXMUTEX -L$(DIR) test.c -o test_posixmutex $(LIBRARY)
test_posix_spin: test.c library
	$(CC) $(CFLAGSNEW) -D_POSIXSPIN -L$(DIR) test.c -o test_posixspin $(LIBRARY)
test_MCS: test.c library
	$(CC) $(CFLAGSNEW) -D_MCS_SPIN -L$(DIR) test.c -o test_MCS $(LIBRARY)
test_adaptive: test.c library
	$(CC) $(CFLAGSNEW) -D_ADAPTIVE -L$(DIR) test.c -o test_adaptive $(LIBRARY)
test: test_spin test_ticket test_mutex test_posix_mutex test_posix_spin test_MCS test_adaptive
	@true

#compile test files to get metric_LOCK apps
test_spin_metric: test.c library
	$(CC) $(CFLAGSNEW) -D_SPINCLASSIC -D_METRIC -L$(DIR) test.c -o metric_spin $(LIBRARY)
test_ticket_metric: test.c library
	$(CC) $(CFLAGSNEW) -D_TICKETSPIN -D_METRIC -L$(DIR) test.c -o metric_ticket $(LIBRARY)
test_mutex_metric: test.c library
	$(CC) $(CFLAGSNEW) -D_MUTEX -D_METRIC -L$(DIR) test.c -o metric_mutex $(LIBRARY)
test_posix_mutex_metric: test.c library
	$(CC) $(CFLAGSNEW) -D_POSIXMUTEX -D_METRIC -L$(DIR) test.c -o metric_posixmutex $(LIBRARY)
test_posix_spin_metric: test.c library
	$(CC) $(CFLAGSNEW) -D_POSIXSPIN -D_METRIC -L$(DIR) test.c -o metric_posixspin $(LIBRARY)
test_MCS_metric: test.c library
	$(CC) $(CFLAGSNEW) -D_MCS_SPIN -D_METRIC -L$(DIR) test.c -o metric_MCS $(LIBRARY)
test_adaptive_metric: test.c library
	$(CC) $(CFLAGSNEW) -D_ADAPTIVE -D_METRIC -L$(DIR) test.c -o metric_adaptive $(LIBRARY)
test_metric: test_spin_metric test_ticket_metric test_mutex_metric test_posix_mutex_metric test_posix_spin_metric test_MCS_metric test_adaptive_metric
	@true

#everything
all: test test_time test_metric
	@true

#test staff
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
