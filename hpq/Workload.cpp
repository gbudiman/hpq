//
//  Workload.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/5/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "Workload.hpp"
using namespace std;

Workload::Workload() {
  initialize_workload(DEFAULT_WORKLOAD_THREADS);
}

Workload::Workload(int n) {
  initialize_workload(n);
}

void Workload::initialize_workload(int n) {
  loaders = vector<thread>(n);
  
  for (int i = 0; i < n; i++) {
    //thread t = move(thread(&Workload::run, i));
    loaders.at(i) = move(thread(&Workload::run, i));
  }

  for (int i = 0; i < n; i++) {
    loaders.at(i).join();
  }
}

void Workload::run(int id) {
  printf("Thread %d initialized\n", id);
  this_thread::sleep_for(chrono::microseconds(LOAD_DELAYED_START));
  for (int i = 0; i < LOAD_ITERATION_LIMIT; i++) {
    printf("Thread %d Running\n", id);
    this_thread::sleep_for(chrono::microseconds(LOAD_SLEEP));
  }
}
