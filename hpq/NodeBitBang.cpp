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

long int NodeBitBang::get() {
  if (is_marked()) { return BITBANG_INVALID; }
  
  return data;
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
  if (owner == 255) return true;
  return owner == thread_id;
}

bool NodeBitBang::is_marked() {
  return (data.load()      ) &        0x1;
}

long int NodeBitBang::wrap(uint16_t priority, uint8_t thread_id) {
  return (priority << 16) | (thread_id << 8);
}

void NodeBitBang::lock(uint8_t thread_id) {
  bool lockable = (owner() == 0xFF || owner() == thread_id);
  
  while (!lockable) {
    lockable = (owner() == 0xFF || owner() == thread_id);
    printf("Thread: %d spin wait for lock, owner is thread %d\n", thread_id, owner());
    this_thread::sleep_for(chrono::milliseconds(200));
  }
  
  long int thread_mask = 0xFFFFFFFFFFFF00FF | (thread_id << 8);
  data.store(data.load() & thread_mask);
}

void NodeBitBang::release(uint8_t thread_id) {
  if (!is_owner(thread_id)) { throw "Thread access violation in NodeBitBang::release"; }
  data.store(data.load() | (0xFF << 8));
}
