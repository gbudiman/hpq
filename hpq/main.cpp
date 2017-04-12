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
#include "HeapPriorityDistributed2.hpp"
using namespace std;

HeapPriorityBasic<int> h = HeapPriorityBasic<int>();
HeapPriorityDistributed2 h2 = HeapPriorityDistributed2();
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

void run_h2(int id) {
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
        h2.put(id, priority);
        printf("   %5d\n", priority);
        
        if (DO_VALIDATE) {
          lock_hputs.lock();
          //cv.record(OP_PUT, priority);
          lock_hputs.unlock();
        }
        count_put++;
        
        break;
      case 1:
        out = h2.take_priority();
        printf(">> %10d\n", out);
        
        if (DO_VALIDATE) {
          lock_htakes.lock();
          //cv.record(OP_TAKE, out);
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
  
  /*for (int i = 0; i < htakes.size(); i++) {
    printf("%d\n", htakes.at(i));
  }*/
  //cv.verify_all();
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

void sequential_stress_test() {
  auto begin = chrono::high_resolution_clock::now();
  int count_put = 0;
  int count_take = 0;
  int seq_limit = DEFAULT_WORKLOAD_THREADS * LOAD_ITERATION_LIMIT;
  
  this_thread::sleep_for(chrono::microseconds(LOAD_DELAYED_START));
  if (SHOW_PROGRESS) { printf("Thread %3d started\n", -1); }
  for (int i = 0; i < seq_limit; i++) {
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
    
    if (SHOW_PROGRESS && i % (seq_limit / 10) == 0) {
      printf("Thread %3d: %3.0f%% (%6d / %6d)\n", -1, (float) i / seq_limit * 100.0f, count_put, count_take);
    }
    this_thread::sleep_for(chrono::microseconds(LOAD_SLEEP == 0 ? 0 : rand() % LOAD_SLEEP));
  }
  
  auto end = chrono::high_resolution_clock::now();
  auto time_diff = chrono::duration_cast<chrono::milliseconds>(end - begin);
  printf("Run takes %.2lld ms\n", time_diff.count());
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
  printf("Run takes %.2lld ms\n", time_diff.count());
}

void threaded_stress_test_distributed2() {
  vector<thread> threads = vector<thread>();
  
  auto begin = chrono::high_resolution_clock::now();
  for (int i = 0; i < DEFAULT_WORKLOAD_THREADS; i++) {
    threads.push_back(thread(run_h2, i));
  }
  
  for (int i = 0; i < DEFAULT_WORKLOAD_THREADS; i++) {
    threads.at(i).join();
  }
  
  auto end = chrono::high_resolution_clock::now();
  h2._verify_all();
  
  auto time_diff = chrono::duration_cast<chrono::milliseconds>(end - begin);
  printf("Run takes %.2lld ms\n", time_diff.count());
}

int main(int argc, const char * argv[]) {
  srand(time(NULL));
  //Workload();
  //UnitTest();
  //sequential_stress_test();
  //threaded_stress_test();
  threaded_stress_test_distributed2();
}



