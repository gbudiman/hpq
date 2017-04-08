//
//  Workload.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/5/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "Workload.hpp"
using namespace std;

Workload::Workload(HeapPriorityBasic<int> h) {
  initialize_workload(h, DEFAULT_WORKLOAD_THREADS);
}

Workload::Workload(HeapPriorityBasic<int> h, int n) {
  initialize_workload(h, n);
}

void Workload::initialize_workload(HeapPriorityBasic<int> h, int n) {
  srand(time(NULL));
  //auto t0 = thread(&Workload::run, 0);
  //t0.join();
  /*loaders = vector<thread>(n);
  this->h = h;
  
  for (int i = 0; i < n; i++) {
    //thread t = move(thread(&Workload::run, i));
    loaders.at(i) = move(thread(&Workload::run, i));
  }

  for (int i = 0; i < n; i++) {
    loaders.at(i).join();
  }*/
}

void Workload::run(int id) {
  printf("Thread %d initialized\n", id);
  this_thread::sleep_for(chrono::microseconds (LOAD_DELAYED_START));
//  for (int i = 0; i < LOAD_ITERATION_LIMIT; i++) {
//    printf("Thread %d Running\n", id);
//    this_thread::sleep_for(chrono::microseconds(LOAD_SLEEP));
//  }
  
  vector<int> puts = vector<int>();
  vector<int> takes = vector<int>();
  vector<int> diff = vector<int>();
  
  for (int i = 0; i < LOAD_ITERATION_LIMIT; i++) {
    int operation = Workload::random_operation();
    int priority;
    int out;
    
    switch(operation) {
      case 0:
        priority = Workload::random_priority();
        h.put(priority);
        puts.push_back(priority);
        //s += " -> " + to_string(priority) + "\n";
        break;
      case 1:
        out = h.take_priority();
        takes.push_back(out);
        //s += " <- " + to_string(out) + "\n";
        break;
    }
  }
}

int Workload::random_priority() {
  return rand() % (LOAD_PRIORITY_MAX - LOAD_PRIORITY_MIN) + LOAD_PRIORITY_MIN;
}

int Workload::random_operation() {
  // Returns 1 = take();
  //         0 = put();
  int threshold = LOAD_PUT_TO_TAKE_RATIO * RAND_MAX;
  return rand() < threshold ? 0 : 1;
}
