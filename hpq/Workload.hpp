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


class Workload {
public:
  Workload(HeapPriorityBasic<int>);
  Workload(HeapPriorityBasic<int>, int n);
  void run(int id);
  static int random_priority();
  static int random_operation();
  
private:
  void initialize_workload(HeapPriorityBasic<int>, int n);
  std::vector<std::thread> loaders;
  HeapPriorityBasic<int> h;
};

#endif /* Workload_hpp */
