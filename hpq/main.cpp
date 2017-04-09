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
#include "HeapPriorityConcurrent.hpp"
using namespace std;

HeapPriorityBasic<int> h = HeapPriorityBasic<int>();
HeapPriorityConcurrent<int> hc = HeapPriorityConcurrent<int>();
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
  
  this_thread::sleep_for(chrono::microseconds(LOAD_DELAYED_START));
  if (SHOW_PROGRESS) { printf("Thread %3d started\n", id); }
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
        
        if (DO_VALIDATE) {
          lock_hputs.lock();
          hputs.push_back(priority);
          lock_hputs.unlock();
        }
        count_put++;
        
        break;
      case 1:
        lock_h.lock();
        out = h.take_priority();
        lock_h.unlock();
        
        if (DO_VALIDATE) {
          lock_htakes.lock();
          htakes.push_back(out);
          lock_htakes.unlock();
        }
        count_take++;
        break;
    }
    
    if (SHOW_PROGRESS && i % (LOAD_ITERATION_LIMIT / 10) == 0) {
      printf("Thread %3d: %3.0f%% (%6d / %6d)\n", id, (float) i / LOAD_ITERATION_LIMIT * 100.0f, count_put, count_take);
    }
    this_thread::sleep_for(chrono::microseconds(LOAD_SLEEP == 0 ? 0 : rand() % LOAD_SLEEP));
  }
  
  printf("Thread %3d completed after %d/%d put/take\n", id, count_put, count_take);
}

void run_fine_grained(int id) {
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
        hc.put(priority);
        lock_h.unlock();
        
        if (DO_VALIDATE) {
          lock_hputs.lock();
          hputs.push_back(priority);
          lock_hputs.unlock();
        }
        count_put++;
        
        break;
      case 1:
        lock_h.lock();
        out = hc.take_priority();
        lock_h.unlock();
        
        if (DO_VALIDATE) {
          lock_htakes.lock();
          htakes.push_back(out);
          lock_htakes.unlock();
        }
        count_take++;
        break;
    }
  }
}

void verify_threaded_run() {
  sort(hputs.begin(), hputs.end());
  sort(htakes.begin(), htakes.end());
  set_difference(hputs.begin(), hputs.end(), htakes.begin(), htakes.end(), inserter(hdiff, hdiff.begin()));
  
  if (h.identical_contents(hdiff)) {
    cout << "Threaded run content matched" << endl;
  } else {
    cout << "Threaded run content MISMATCHED" << endl;
  }
}

void threaded_stress_test() {
  vector<thread> threads = vector<thread>();
  
  auto begin = chrono::high_resolution_clock::now();
  for (int i = 0; i < DEFAULT_WORKLOAD_THREADS; i++) {
    threads.push_back(thread(run, i));
  }
  
  for (int i = 0; i < DEFAULT_WORKLOAD_THREADS; i++) {
    threads.at(i).join();
  }
  
  auto end = chrono::high_resolution_clock::now();
  h._verify_all();
  if (DO_VALIDATE) { verify_threaded_run(); }
  
  auto time_diff = chrono::duration_cast<chrono::milliseconds>(end - begin);
  printf("Run takes %.2llu ms\n", time_diff);
}

void threaded_stress_test_concurrent() {
  vector<thread> threads = vector<thread>();
  
  for (int i = 0; i < DEFAULT_WORKLOAD_THREADS; i++) {
    threads.push_back(thread(run_fine_grained, i));
  }
  
  for (int i = 0; i < DEFAULT_WORKLOAD_THREADS; i++) {
    threads.at(i).join();
  }
  
  hc._verify_all();
}

int main(int argc, const char * argv[]) {
  srand(time(NULL));
  //Workload();
  //UnitTest();
  
  //threaded_stress_test();
  UnitTest();
}



