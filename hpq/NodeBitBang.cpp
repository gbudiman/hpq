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
  long int other_data;

  try {
    other_data = other.data.load();
  } catch(int e) {
    printf("Exception here\n");
    
    other_data = DEFAULT_NODE;
  }

  data.store(other_data);
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
  if (this->owner() == 0xFF) return true;
  return this->owner() == thread_id;
}

bool NodeBitBang::is_marked() {
  return (data.load()      ) &        0x1;
}

long int NodeBitBang::wrap(uint16_t priority, uint8_t thread_id) {
  return (priority << 16) | (thread_id << 8);
}

bool NodeBitBang::lock(uint8_t thread_id) {
  auto old_data = data.load();
  uint8_t stored_owner = (old_data & 0xFF00) >> 8;
  
  if (stored_owner == 0xFF || stored_owner == thread_id) {
    long int thread_mask = 0xFFFFFFFFFFFF00FF;
    long int new_data = (old_data & thread_mask) + (thread_id << 8);
    
    if (data.compare_exchange_strong(old_data, new_data)) {
      printf("         Thread %d succeeded (%lX)\n", thread_id, old_data);
      return true;
    }
  }
  
  printf("         Thread %d failed (%lX)\n", thread_id, old_data);
  return false;
  
  /*bool lockable = (owner() == 0xFF || owner() == thread_id);
    //printf("Thread: %d spin wait for lock, owner is thread %d\n", thread_id, owner());
    //this_thread::sleep_for(chrono::milliseconds(200));
  //}
  
  if (lockable) {
    long int thread_mask = 0xFFFFFFFFFFFF00FF;
    long int old_data = data.load();
    long int new_data = (old_data & thread_mask) + (thread_id << 8);
    //data.store(data.load() & thread_mask);
    
    
    data.store(new_data);
    return true;
    if (data.compare_exchange_strong(old_data, new_data)) {
      return true;
    }
    
    //return false;
  }
  
  return false;*/
}

void NodeBitBang::release(uint8_t thread_id) {
  long int new_data = data.load() | (0xFF << 8);
  
  if (is_owner(thread_id)) {
    data.store(new_data);
    printf("^^^ Thread %d released lock (new_data = %lX)\n", thread_id, new_data);
  } else {
    printf("!!! Thread %d violated access to node locked by Thread %d\n", thread_id, owner());
    
    throw -2;
  }
}

/*void NodeBitBang::release(uint8_t thread_id, int mode) {
  //if (!is_owner(thread_id)) { throw "Thread access violation in NodeBitBang::release"; }
  if (!is_owner(thread_id)) {
    if (mode == UNCONDITIONAL_RELEASE) {
      //printf("!!! Thread %d unconditionally released lock\n", thread_id);
      data.store(data.load() | (0xFF << 8));
    } else {
      printf("!!! Thread %d violated access to node locked by Thread %d\n", thread_id, owner());
      throw -2;
    }
  } else {
    data.store(data.load() | (0xFF << 8));
  }
}*/
