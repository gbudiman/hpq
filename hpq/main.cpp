//
//  main.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/5/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "main.hpp"
#include "Workload.hpp"
#include "UnitTest.hpp"
#include "HeapPriorityBasic.hpp"
using namespace std;

HeapPriorityBasic<int> h = HeapPriorityBasic<int>();
vector<int> hputs = vector<int>();
vector<int> htakes = vector<int>();
vector<int> hdiff = vector<int>();
mutex lock_hputs;
mutex lock_htakes;
mutex lock_hdiff;
mutex lock_h;

void run(int id) {
  int count_put = 0;
  int count_take = 0;
  for (int i = 0; i < LOAD_ITERATION_LIMIT; i++) {
    int operation = Workload::random_operation();
    int priority;
    int out;
    
    switch(operation) {
      case 0:
        priority = Workload::random_priority();
        lock_h.lock();
        h.put(priority);
        lock_h.unlock();
        
        lock_hputs.lock();
        hputs.push_back(priority);
        lock_hputs.unlock();
     
        count_put++;
        break;
      case 1:
        lock_h.lock();
        out = h.take_priority();
        lock_h.unlock();
        
        lock_htakes.lock();
        htakes.push_back(out);
        lock_htakes.unlock();
        
        count_take++;
        break;
    }
    
    this_thread::sleep_for(chrono::microseconds(LOAD_SLEEP));
  }
  
  printf("Thread %d completed after %d/%d put/take\n", id, count_put, count_take);
}

int main(int argc, const char * argv[]) {
  //Workload();
  //UnitTest();
  
  
  vector<thread> threads = vector<thread>();
  
  for (int i = 0; i < 32; i++) {
    threads.push_back(move(thread(run, i)));
  }
  
  for (int i = 0;i < 32; i++) {
    threads.at(i).join();
  }
  
  //h.debug_print();
  h._verify_all();
}

