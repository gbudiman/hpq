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
