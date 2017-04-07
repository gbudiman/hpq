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
  
  bool equals(std::vector<int> cmps);
  bool weak_equals(std::vector<int> cmps);
  
  void debug_print();
  int get_height();
  int get_parent_priority(int i);
  void _get_parent_priority(int i);
  std::tuple<int, int> get_children_priority(int i);
  void _get_children_priority(int i);
  int get_priority_at(int i);
  void _verify_all();
  bool verify_all();
  bool verify_all(std::vector<int> cmps);
  bool verify(int i);
  
  int peek_priority();
  int take_priority();
  std::tuple<T, int> peek();
  std::tuple<T, int> take();
  HeapPriorityBasic<T> remove();
  //void put(std::tuple<T, int> in);
  HeapPriorityBasic<T> put(int i);
  int size();
private:
  std::vector<std::tuple<T, int>> data;
  void initialize_data();
  void last_to_first();
  void sift(int i);
  void swap(int a, int b);
  void percolate(int i);
  std::tuple<T, int> apply_removal();
  bool is_empty();
  
};

#endif /* HeapPriorityBasic_hpp */
