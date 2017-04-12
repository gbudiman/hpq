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

class HeapPriorityDistributed2 {
public:
  HeapPriorityDistributed2();
  HeapPriorityDistributed2(int bins);
  HeapPriorityDistributed2(const HeapPriorityDistributed2& other);
  HeapPriorityDistributed2 put(int thread_id, int priority);
  Node<int> take();
  int take_priority();
  
  bool verify_all();
  void _verify_all();
  
private:
  void initialize(int bins);
  //int get_minbin_index();
  void update_minbin(int index, int val);
  void update_global_minbin();
  //void invalidate_minbin(int index);
  
  bool verify_bin(int index);
  
  std::vector<HeapPriorityBasic<int>> dist;
  std::vector<int> minbin;
  //std::tuple<int, int> global_minbin; // <index, value>
  int global_minbin; // index
  
  std::vector<std::mutex> mutexes;
  std::mutex minbin_mutex;
  
  std::mutex debug_print_mutex;
};
#endif /* HeapPriorityDistributed2_hpp */
