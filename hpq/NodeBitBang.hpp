//
//  NodeBitBang.hpp
//  hpq
//
//  Created by Gloria Budiman on 4/9/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#ifndef NodeBitBang_hpp
#define NodeBitBang_hpp

#define DEFAULT_NODE 0xFFFFFFFF00000000

#include "main.hpp"

class NodeBitBang {
public:
  NodeBitBang();
  NodeBitBang(const NodeBitBang& other);
  NodeBitBang(uint16_t priority);
  
  uint32_t item();
  uint16_t priority();
  uint8_t owner();
  bool is_owner(uint8_t thread_id);
  bool is_marked();
  
  std::atomic<long int> data;
};

#endif /* NodeBitBang_hpp */
