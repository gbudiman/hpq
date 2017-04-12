//
//  RunnerThread.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/11/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "RunnerThread.hpp"
using namespace std;

RunnerThread::RunnerThread(shared_ptr<HeapPriorityBasic<int>> hb,
                           shared_ptr<HeapPriorityDistributed2> h2,
                           int num_threads, int limit,
                           shared_ptr<ConcurrentVerificator> cv) {
  this->num_threads = num_threads;
  
  if (hb != NULL) {
    run_mode = RUN_BASIC;
    this->hb = hb;
  } else if (h2 != NULL) {
    run_mode = RUN_H2;
    this->h2 = h2;
  }

  this->limit = limit;
  this->cv = cv;
  
  count_take.store(0);
  count_put.store(0);
}

RunnerThread::RunnerThread(const RunnerThread& other) {
  
}

tuple<int, int> RunnerThread::run() {
  vector<thread> threads = vector<thread>();
  
  printf("---\n");
  
  for (int i = 0; i < num_threads; i++) {
    threads.push_back(thread(&RunnerThread::run_task, this, i, limit));
  }
  
  for (int i = 0; i < num_threads; i++) {
    threads.at(i).join();
  }
  
  return tuple<int, int>(count_put.load(), count_take.load());
}

void RunnerThread::run_task(int thread_id, int limit) {
  int local_count_put = 0;
  int local_count_take = 0;
  
  for (int i = 0; i < limit; i++) {
    int operation = RunnerThread::random_operation();
    int priority;
    int out;
    
    switch(operation) {
      case 0:
        priority = RunnerThread::random_priority();
        
        if (run_mode == RUN_BASIC) {
          lock_h.lock();
          hb->put(priority);
          
          lock_h.unlock();
        } else if (run_mode == RUN_H2) {
          h2->put(thread_id, priority);
        }
        
        cv->record(OP_PUT, priority, thread_id);
        count_put.fetch_add(1);
        local_count_put++;
        break;
      case 1:
        if (run_mode == RUN_BASIC) {
          lock_h.lock();
          out = hb->take_priority();
          
          lock_h.unlock();
        } else if (run_mode == RUN_H2) {
          out = h2->take_priority();
        }
        
        cv->record(OP_TAKE, out, thread_id);
        
        count_take.fetch_add(1);
        local_count_take++;
        break;
    }
    
    if (SHOW_PROGRESS && i % (limit / 10) == 0) {
      printf("Thread %3d: %3.0f%% (%6d / %6d)\n", thread_id, (float) i / limit * 100.0f, local_count_put, local_count_take);
    }
  }
}

int RunnerThread::random_priority() {
  return rand() % (LOAD_PRIORITY_MAX - LOAD_PRIORITY_MIN) + LOAD_PRIORITY_MIN;
}

int RunnerThread::random_operation() {
  // Returns 1 = take();
  //         0 = put();
  int threshold = LOAD_PUT_TO_TAKE_RATIO * RAND_MAX;
  return rand() < threshold ? 0 : 1;
}
