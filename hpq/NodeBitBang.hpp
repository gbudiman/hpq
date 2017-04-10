//
//  NodeBitBang.hpp
//  hpq
//
//  Created by Gloria Budiman on 4/9/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#ifndef NodeBitBang_hpp
#define NodeBitBang_hpp

#include "main.hpp"

class NodeBitBang {
public:
  NodeBitBang();
  NodeBitBang(const NodeBitBang& other);
  long int get();
  
  NodeBitBang(uint16_t priority);
  
  static long int wrap(uint16_t priority, uint8_t thread_id);
  
  uint32_t item();
  uint16_t priority();
  uint8_t owner();
  bool is_owner(uint8_t thread_id);
  bool is_marked();
  
  bool lock(uint8_t thread_id);
  //void release(uint8_t thread_id, int mode);
  void release(uint8_t thread_id);
  
  std::atomic<long int> data;
};

#endif /* NodeBitBang_hpp */
