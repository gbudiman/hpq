//
//  NodeBitBang.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/9/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "NodeBitBang.hpp"
using namespace std;

NodeBitBang::NodeBitBang() {
  data.store(DEFAULT_NODE);
}

NodeBitBang::NodeBitBang(uint16_t priority) {
  data.store(priority << 16);
}

NodeBitBang::NodeBitBang(const NodeBitBang& other) {
  data.store(other.data.load());
}

uint32_t NodeBitBang::item() {
  return (data.load() >> 32) & 0xFFFFFFFF;
}

uint16_t NodeBitBang::priority() {
  return (data.load() >> 16) &     0xFFFF;
}

uint8_t NodeBitBang::owner() {
  return (data.load() >>  8) &       0xFF;
}

bool NodeBitBang::is_owner(uint8_t thread_id) {
  uint8_t owner = this->owner();
  if (owner == 255) return false;
  return owner == thread_id;
}

bool NodeBitBang::is_marked() {
  return (data.load()      ) &        0x1;
}
