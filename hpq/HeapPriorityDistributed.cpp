//
//  HeapPriorityDistributed.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/10/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "HeapPriorityDistributed.hpp"
using namespace std;

HeapPriorityDistributed::HeapPriorityDistributed() {
  dist = unordered_map<int, HeapPriorityBasic<int>>();
}

HeapPriorityDistributed::HeapPriorityDistributed(const HeapPriorityDistributed& other) {
  
}

HeapPriorityDistributed HeapPriorityDistributed::put(int p, int thread_id) {
  if (dist.find(thread_id) == dist.end()) {
    // fresh meat
    allocate_bin(thread_id);
    printf("Bin for T%d allocated\n", thread_id);
  }
  
  while (busy.at(thread_id).is_busy()) {
    //printf("Thread %d spin wait put\n", thread_id);
  }
  busy.at(thread_id).set_busy();
  dist.at(thread_id).put(p);
  busy.at(thread_id).set_free();
  
  return *this;
}

void HeapPriorityDistributed::allocate_bin(int thread_id) {
  HeapPriorityDistributed::overlord_mutex.lock();
  dist.insert(pair<int, HeapPriorityBasic<int>>(thread_id, HeapPriorityBasic<int>()));
  busy.insert(pair<int, AtomicWrapper>(thread_id, AtomicWrapper()));
  HeapPriorityDistributed::overlord_mutex.unlock();
}

void HeapPriorityDistributed::debug_print() {
  for (auto it = dist.begin(); it != dist.end(); ++it) {
    printf("Bin %3d:\n", it->first);
    it->second.debug_print();
  }
}

void HeapPriorityDistributed::_verify_all() {
  if (verify_all()) {
    printf("%lu bins, all valid\n", dist.size());
  } else {
    printf("Invalid heap\n");
  }
}

bool HeapPriorityDistributed::verify_all() {
  bool result = true;
  
  for (auto it = dist.begin(); it != dist.end(); ++it) {
    auto this_bin = it->second.verify_all();
    result &= this_bin;
    
    if (!this_bin) {
      printf("Invalid bin %d\n", it->first);
      it->second.debug_print();
    }
  }
  
  return result;
}

Node<int> HeapPriorityDistributed::peek() {
  return dist.at(get_min_dist_index()).peek();
}

Node<int> HeapPriorityDistributed::take(int thread_id) {
  auto index = get_min_dist_index();
  
  if (dist.find(index) == dist.end()) {
    allocate_bin(thread_id);
    return Node<int>();
  }
  auto result = dist.at(index).peek();
  
  while (busy.at(thread_id).is_busy()) {
    //printf("Thread %d spin wait take\n", thread_id);
  }
  
  busy.at(index).set_busy();
  dist.at(index).remove();
  busy.at(index).set_free();
  return result;
}

int HeapPriorityDistributed::take_priority(int thread_id) {
  return take(thread_id).priority;
}

int HeapPriorityDistributed::get_min_dist_index() {
  auto s = peek_dist();
  
  int min = __INT_MAX__;
  int index = -1;
  for (auto it = s.begin(); it != s.end(); ++it) {
    int element_value = it->second.priority;
    
    if (element_value < min) {
      min = element_value;
      index = it->first;
    }
  }
  
  return index;
}

unordered_map<int, Node<int>> HeapPriorityDistributed::peek_dist() {
  auto s = unordered_map<int, Node<int>>();
  auto has_been_collected = unordered_map<int, bool>();
  
  overlord_mutex.lock();
  for (auto it = dist.begin(); it != dist.end(); ++it) {
    s.insert(pair<int, Node<int>>(it->first, NULL));
    has_been_collected.insert(pair<int, bool>(it->first, false));
  }
  overlord_mutex.unlock();
  
  int free_count = 0;
  
  while (free_count < dist.size()) {
    free_count = 0;
    for (auto it = has_been_collected.begin(); it != has_been_collected.end(); ++it) {
      int thread_id = it->first;
      bool collect_status = it->second;
      
      if (collect_status) {
        free_count++;
        continue;
      }
      
      if (!busy.at(thread_id).is_busy()) {
        it->second = true;
        s.at(thread_id) = dist.at(thread_id).peek();
      }
    }
  }
  
  return s;
}
