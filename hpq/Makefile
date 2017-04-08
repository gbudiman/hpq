CC=g++
CF=-g -std=c++11 -Wall -pedantic -pthread

all: workload hpqb hpqc unit_test node
	$(CC) $(CF) *.o main.hpp main.cpp

workload:
	$(CC) $(CF) -c Workload.cpp

hpqb:
	$(CC) $(CF) -c HeapPriorityBasic.cpp

hpqc:
	$(CC) $(CF) -c HeapPriorityConcurrent.cpp

unit_test:
	$(CC) $(CF) -c UnitTest.cpp

node:
	$(CC) $(CF) -c Node.cpp

clean:
	rm *.o a.out *.gch