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
#include "ConcurrentVerificator.hpp"
using namespace std;

int main(int argc, const char * argv[]) {
  srand(time(NULL));
  //UnitTest();
  auto t_seq = Workload::run_sequential(DEFAULT_WORKLOAD_THREADS * LOAD_ITERATION_LIMIT);
  auto t_cgh = Workload::run_coarse_grained(DEFAULT_WORKLOAD_THREADS, LOAD_ITERATION_LIMIT);
  auto t_ch2 = Workload::run_concurrent(DEFAULT_WORKLOAD_THREADS, LOAD_ITERATION_LIMIT);
  Workload::summarize(t_seq, t_cgh, t_ch2);
}



