//
//  HeapPriorityDistributed.hpp
//  hpq
//
//  Created by Gloria Budiman on 4/10/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#ifndef HeapPriorityDistributed_hpp
#define HeapPriorityDistributed_hpp

#include "main.hpp"
#include "HeapPriorityBasic.hpp"
#include "Node.hpp"
#include "AtomicWrapper.hpp"

class HeapPriorityDistributed {
public:
  HeapPriorityDistributed();
  HeapPriorityDistributed(const HeapPriorityDistributed& other);
  
  HeapPriorityDistributed put(int p, int thread_id);
  Node<int> take(int thread_id);
  int take_priority(int thread_id);
  Node<int> peek();
  HeapPriorityDistributed remove();
  void debug_print();
  void _verify_all();
  bool verify_all();
  std::tuple<bool, int> verify_all_with_size();
  
  std::mutex overlord_mutex;
private:
  int get_min_dist_index();
  std::unordered_map<int, Node<int>> peek_dist();
  
  HeapPriorityBasic<int> dist_safe_access(int id);
  AtomicWrapper busy_safe_access(int id);
  
  std::unordered_map<int, HeapPriorityBasic<int>> dist;
  std::unordered_map<int, AtomicWrapper> busy;
  void allocate_bin(int thread_id);
  
  
};
#endif /* HeapPriorityDistributed_hpp */
