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
  
private:
  std::vector<std::tuple<T, int>> data;
  void initialize_data();
  
};

#endif /* HeapPriorityBasic_hpp */
