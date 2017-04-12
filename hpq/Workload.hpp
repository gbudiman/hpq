//
//  Workload.hpp
//  hpq
//
//  Created by Gloria Budiman on 4/5/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#ifndef Workload_hpp
#define Workload_hpp

#include "main.hpp"
#include "HeapPriorityBasic.hpp"
#include "HeapPriorityDistributed2.hpp"
#include "RunnerThread.hpp"

class Workload {
public:
  static int random_priority();
  static int random_operation();
  
  static long int run_sequential(int limit);
  static long int run_coarse_grained(int threads, int limit);
  static long int run_concurrent(int threads, int limit);
  
  static void summarize(long int t_seq, long int t_cgh, long int t_ch2);
  
private:
  static std::tuple<int, int> run(std::shared_ptr<HeapPriorityBasic<int>> h, int thread_id, int limit);
  static long int time_it(std::chrono::time_point<std::chrono::high_resolution_clock> a, std::chrono::time_point<std::chrono::high_resolution_clock> b);
  
  std::shared_ptr<HeapPriorityBasic<int>> hb;
};

#endif /* Workload_hpp */
