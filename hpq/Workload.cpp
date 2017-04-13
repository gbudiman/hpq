//
//  Workload.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/5/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "Workload.hpp"
using namespace std;

long int Workload::run_sequential(int limit) {
  auto h = make_shared<HeapPriorityBasic<int>>();
  auto cv = make_shared<ConcurrentVerificator>();
  auto rt = RunnerThread(h, NULL, cv);
  auto begin = chrono::high_resolution_clock::now();
  auto tracker = rt.run();
  auto end = chrono::high_resolution_clock::now();
  
  printf("Sequential run completed        %6dP / %6dT\n", get<0>(tracker), get<1>(tracker));
  
  if (verify_correctness) cv->done();
  return time_it(begin, end);
}

long int Workload::run_coarse_grained() {
  vector<RunnerThread> threads = vector<RunnerThread>();
  auto h = make_shared<HeapPriorityBasic<int>>();
  auto cv = make_shared<ConcurrentVerificator>();
  auto rt = RunnerThread(h, NULL, cv);
  
  auto begin = chrono::high_resolution_clock::now();
  auto tracker = rt.run();
  auto end = chrono::high_resolution_clock::now();
  
  printf("Coarse run completed     (%3dT) %6dP / %6dT\n", num_threads, get<0>(tracker), get<1>(tracker));
  
  cv->done();
  return time_it(begin, end);
}

long int Workload::run_concurrent() {
  vector<RunnerThread> threads = vector<RunnerThread>();
  auto h = make_shared<HeapPriorityDistributed2>(num_threads);
  auto cv = make_shared<ConcurrentVerificator>();
  auto rt = RunnerThread(NULL, h, cv);
  
  h->attach_verificator(cv);
  
  auto begin = chrono::high_resolution_clock::now();
  auto tracker = rt.run();
  auto end = chrono::high_resolution_clock::now();
  
  printf("Concurrent run completed (%3dT) %6dP / %6dT\n", num_threads, get<0>(tracker), get<1>(tracker));
  
  cv->done();
  return time_it(begin, end);
}

long int Workload::time_it(std::chrono::time_point<std::chrono::high_resolution_clock> a, std::chrono::time_point<std::chrono::high_resolution_clock> b) {
  chrono::milliseconds diff = chrono::duration_cast<chrono::milliseconds>(b - a);
  
  return (long int) diff.count();
}

void Workload::summarize(long int t_seq, long int t_cgh, long int t_ch2) {
  float ops =  (float) iteration_limit * num_threads;
  printf("Sequential run:     %8ld ms (%8.2f kOPS/s)\n", t_seq, ops/t_seq);
  printf("Coarse-grained run: %8ld ms (%8.2f kOPS/s)\n", t_cgh, ops/t_cgh);
  printf("Concurrent run:     %8ld ms (%8.2f kOPS/s)\n", t_ch2, ops/t_ch2);
}
