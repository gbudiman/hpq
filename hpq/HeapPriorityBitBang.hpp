//
//  HeapPriorityBitBang.hpp
//  hpq
//
//  Created by Gloria Budiman on 4/9/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#ifndef HeapPriorityBitBang_hpp
#define HeapPriorityBitBang_hpp

#include "main.hpp"
#include "NodeBitBang.hpp"
using namespace std;

class HeapPriorityBitBang {
public:
  HeapPriorityBitBang();
  
  HeapPriorityBitBang put(uint16_t priority, uint8_t thread_id);
  //HeapPriorityBitBang put(uint32_t item, uint16_t priority);
  int size();
  
  uint16_t take_priority();
  
  int get_height();
  uint16_t get_priority_at(int i);
  
  bool verify(int i);
  bool verify_all();
  void _verify_all();
  void debug_print();
  
private:
  vector<NodeBitBang> data;
  
  void initiallize_data();
  void percolate(int i, uint8_t thread_id);
  void swap(int a, int b, uint8_t thread_id);
  
  void lock(int i, uint8_t thread_id);
  void release(int i, uint8_t thread_id);
  
  uint16_t get_parent_priority(int i);
  tuple<uint16_t, uint16_t> get_children_priority(int i);
};

#endif /* HeapPriorityBitBang_hpp */
