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
  
  HeapPriorityBitBang put(uint16_t priority);
  //HeapPriorityBitBang put(uint32_t item, uint16_t priority);
  
  void debug_print();
  
private:
  vector<NodeBitBang> data;
  
  void initiallize_data();
};

#endif /* HeapPriorityBitBang_hpp */
