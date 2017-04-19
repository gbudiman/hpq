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
                           shared_ptr<ConcurrentVerificator> cv) {
  
  if (hb != NULL) {
    run_mode = RUN_BASIC;
    this->hb = hb;
  } else if (h2 != NULL) {
    run_mode = RUN_H2;
    this->h2 = h2;
  }

  this->cv = cv;
  
  count_take.store(0);
  count_put.store(0);
}

RunnerThread::RunnerThread(const RunnerThread& other) {
  // copy constructor so compiler doesn't complain on deleted mutex/atomic constructors
}

tuple<int, int> RunnerThread::run() {
  vector<thread> threads = vector<thread>();
  
  for (int i = 0; i < num_threads; i++) {
    threads.push_back(thread(&RunnerThread::run_task, this, i, iteration_limit));
  }
  
  for (int i = 0; i < num_threads; i++) {
    threads.at(i).join();
  }
  
  return tuple<int, int>(count_put.load(), count_take.load());
}

tuple<int, int> RunnerThread::run(int iteration_limit) {
  run_task(-1, iteration_limit);
  return tuple<int, int>(count_put.load(), count_take.load());
}

void RunnerThread::prepopulate(int n) {
  for (int i = 0; i < n; i++) {
    switch(run_mode) {
      case RUN_BASIC:
        hb->put(RunnerThread::random_priority());
        break;
      case RUN_H2:
        h2->put(i % num_threads, RunnerThread::random_priority());
        break;
    }
  }
}

void RunnerThread::run_task(int thread_id, int limit) {
  int local_count_put = 0;
  int local_count_take = 0;
  
  for (int i = 0; i < limit; i++) {
    int operation = RunnerThread::random_operation(load_ratio);
    int priority;
    int out = -1;
    
    switch(operation) {
      case 0:
        priority = RunnerThread::random_priority();
        
        // sequential implementation of insert
        if (run_mode == RUN_BASIC) {
          lock_h.lock();
          hb->put(priority); // insert data to sequential implementation
                             // adjust the method name accordingly
          lock_h.unlock();
          cv->record(OP_PUT, priority, thread_id); // optional: correctness verification
        } else if (run_mode == RUN_H2) {
          // concurrent implementation insert
          h2->put(thread_id, priority);
        }

        count_put.fetch_add(1);
        local_count_put++;
        break;
      case 1:
        
        // sequential implementation of remove_min
        if (run_mode == RUN_BASIC) {
          lock_h.lock();
          out = hb->take_priority();
          
          lock_h.unlock();
          cv->record(OP_TAKE, out, thread_id);
        } else if (run_mode == RUN_H2) {
          out = h2->take_priority();
        }
        
        count_take.fetch_add(1);
        local_count_take++;
        break;
    }
    
    if (!hide_progress && i % (limit / 10) == 0) {
      printf("Thread %3d: %3.0f%% (%6d / %6d)\n", thread_id, (float) i / limit * 100.0f, local_count_put, local_count_take);
    }
    
    if (usleep > 0) {
      this_thread::sleep_for(chrono::microseconds(load_sleep));
    }
  }
}

int RunnerThread::random_priority() {
  return rand() % (LOAD_PRIORITY_MAX - LOAD_PRIORITY_MIN) + LOAD_PRIORITY_MIN;
}

int RunnerThread::random_operation() {
  return random_operation(LOAD_PUT_TO_TAKE_RATIO * RAND_MAX);
}

int RunnerThread::random_operation(float ratio) {
  // Returns 1 = take();
  //         0 = put();
  int threshold = ratio * RAND_MAX;
  return rand() < threshold ? 0 : 1;
}
