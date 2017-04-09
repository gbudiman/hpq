CC=g++
CF=-g -std=c++11 -Wall -pedantic -pthread

all: workload hpqb unit_test node nodebb hpqbb
	$(CC) $(CF) *.o main.hpp main.cpp

workload:
	$(CC) $(CF) -c Workload.cpp

hpqb:
	$(CC) $(CF) -c HeapPriorityBasic.cpp

hpqbb:
	$(CC) $(CF) -c HeapPriorityBitBang.cpp

unit_test:
	$(CC) $(CF) -c UnitTest.cpp

node:
	$(CC) $(CF) -c Node.cpp

nodebb:
	$(CC) $(CF) -c NodeBitBang.cpp

clean:
	rm *.o a.out *.gch