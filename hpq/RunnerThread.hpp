//
//  RunnerThread.hpp
//  hpq
//
//  Created by Gloria Budiman on 4/11/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#ifndef RunnerThread_hpp
#define RunnerThread_hpp

#define RUN_BASIC 0
#define RUN_H2 1

#include "main.hpp"
#include "HeapPriorityBasic.hpp"
#include "HeapPriorityDistributed2.hpp"
#include "ConcurrentVerificator.hpp"

class RunnerThread {
public:
  RunnerThread(std::shared_ptr<HeapPriorityBasic<int>> hb, std::shared_ptr<HeapPriorityDistributed2> h2, int num_threads, int limit, std::shared_ptr<ConcurrentVerificator> cv);
  RunnerThread(const RunnerThread& other);
  std::tuple<int, int> run();
  
  static int random_priority();
  static int random_operation();
  
private:
  void run_task(int thread_id, int limit);
  
  int run_mode;
  std::shared_ptr<HeapPriorityBasic<int>> hb;
  std::shared_ptr<HeapPriorityDistributed2> h2;
  int num_threads;
  int limit;
  
  std::mutex lock_h;
  
  std::atomic_int count_take;
  std::atomic_int count_put;
  
  std::shared_ptr<ConcurrentVerificator> cv;
};

#endif /* RunnerThread_hpp */
