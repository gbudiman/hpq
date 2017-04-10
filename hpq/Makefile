CC=g++
CF=-g -std=c++11 -Wall -pedantic -pthread

all: workload hpqb unit_test node hpqd atomic
	$(CC) $(CF) *.o main.hpp main.cpp

workload:
	$(CC) $(CF) -c Workload.cpp

hpqb:
	$(CC) $(CF) -c HeapPriorityBasic.cpp

hpqd:
	$(CC) $(CF) -c HeapPriorityDistributed.cpp

unit_test:
	$(CC) $(CF) -c UnitTest.cpp

node:
	$(CC) $(CF) -c Node.cpp

atomic:
	$(CC) $(CF) -c AtomicWrapper.cpp

clean:
	rm *.o a.out *.gch