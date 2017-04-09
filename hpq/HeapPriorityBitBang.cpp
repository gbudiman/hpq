//
//  HeapPriorityBitBang.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/9/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "HeapPriorityBitBang.hpp"
using namespace std;

HeapPriorityBitBang::HeapPriorityBitBang() {
  initiallize_data();
}

HeapPriorityBitBang HeapPriorityBitBang::put(uint16_t priority) {
  long int old = data.at(0).data;
  
  if (data.at(0).data.compare_exchange_strong(old, old)) {
    printf("OK!\n");
  }
  return *this;
}

void HeapPriorityBitBang::initiallize_data() {
  data = vector<NodeBitBang>();
  data.push_back(NodeBitBang());
}

void HeapPriorityBitBang::debug_print() {
  
}
