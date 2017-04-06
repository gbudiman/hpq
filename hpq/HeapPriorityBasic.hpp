//
//  HeapPriorityBasic.hpp
//  hpq
//
//  Created by Gloria Budiman on 4/5/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#ifndef HeapPriorityBasic_hpp
#define HeapPriorityBasic_hpp

#include "main.hpp"

template <class T>
class HeapPriorityBasic {
public:
  HeapPriorityBasic();
  HeapPriorityBasic(std::vector<int> inits);
  
  HeapPriorityBasic<T> load(std::vector<int> inits);
  
  void debug_print();
  int get_height();
  int get_parent_priority(int i);
  void _get_parent_priority(int i);
  std::tuple<int, int> get_children_priority(int i);
  void _get_children_priority(int i);
  int get_priority_at(int i);
  
private:
  std::vector<std::tuple<T, int>> data;
  void initialize_data();
  
};

#endif /* HeapPriorityBasic_hpp */
