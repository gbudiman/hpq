//
//  HeapPriorityDistributed2.hpp
//  hpq
//
//  Created by Gloria Budiman on 4/11/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#ifndef HeapPriorityDistributed2_hpp
#define HeapPriorityDistributed2_hpp

#include "main.hpp"
#include "HeapPriorityBasic.hpp"
#include "Node.hpp"
#include "AtomicMin.hpp"

class HeapPriorityDistributed2 {
public:
  HeapPriorityDistributed2();
  HeapPriorityDistributed2(const HeapPriorityDistributed2& other);
  HeapPriorityDistributed2 put(int thread_id, int priority);
  Node<int> take();
  int take_priority();
  
  bool verify_all();
  void _verify_all();
  
private:
  void initialize(int bins);
  int get_minbin_index();
  void update_minbin(int index, int val);
  
  bool verify_bin(int index);
  
  std::vector<HeapPriorityBasic<int>> dist;
  std::vector<AtomicMin> minbin;
  std::vector<std::mutex> mutexes;
};
#endif /* HeapPriorityDistributed2_hpp */
