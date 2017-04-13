CC=g++
CF=-g -std=c++11 -Wall -pedantic -pthread -Ofast

all: workload hpqb unit_test node h2 runner verificator
	$(CC) $(CF) *.o main.hpp main.cpp

workload:
	$(CC) $(CF) -c Workload.cpp

hpqb:
	$(CC) $(CF) -c HeapPriorityBasic.cpp

h2:
	$(CC) $(CF) -c HeapPriorityDistributed2.cpp

unit_test:
	$(CC) $(CF) -c UnitTest.cpp

node:
	$(CC) $(CF) -c Node.cpp

runner:
	$(CC) $(CF) -c RunnerThread.cpp

verificator:
	$(CC) $(CF) -c ConcurrentVerificator.cpp

clean:
	rm *.o a.out *.gch