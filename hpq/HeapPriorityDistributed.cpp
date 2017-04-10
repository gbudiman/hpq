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
  bool dist_has_been_allocated = (dist.find(thread_id) != dist.end());
  bool busy_has_been_allocated = (busy.find(thread_id) != busy.end());
  
  if (!dist_has_been_allocated || !busy_has_been_allocated) {
    allocate_bin(thread_id);
  }
  
  bool is_busy = true;
  while ((is_busy = busy.at(thread_id).is_busy())) {}
  while(!(is_busy = busy.at(thread_id).set_busy(is_busy))) {}
  dist.at(thread_id).put(p);
  
  while(true) {
    try {
      is_busy = busy.at(thread_id).set_free(is_busy);
      if (!is_busy) { break; }
    } catch (const out_of_range oor) {
      
    }
  }
  //while(!(is_busy = busy.at(thread_id).set_free(is_busy))) {}
  
  return *this;
}

void HeapPriorityDistributed::allocate_bin(int thread_id) {
  HeapPriorityDistributed::overlord_mutex.lock();
  if (dist.find(thread_id) == dist.end()) {
    dist.insert(pair<int, HeapPriorityBasic<int>>(thread_id, HeapPriorityBasic<int>()));
    printf("Distbin for T%d created\n", thread_id);
  } else {
    printf("Skipping distbin for T%d\n", thread_id);
  }
  
  if (busy.find(thread_id) == busy.end()) {
    busy.insert(pair<int, AtomicWrapper>(thread_id, AtomicWrapper()));
    printf("Busybin for T%d created\n", thread_id);
  } else {
    printf("Skipping busybin for T%d\n", thread_id);
  }

  HeapPriorityDistributed::overlord_mutex.unlock();
}

void HeapPriorityDistributed::debug_print() {
  for (auto it = dist.begin(); it != dist.end(); ++it) {
    printf("Bin %3d:\n", it->first);
    it->second.debug_print();
  }
}

void HeapPriorityDistributed::_verify_all() {
  auto verification_result = verify_all_with_size();
  bool result = get<0>(verification_result);
  int sum = get<1>(verification_result);
  
  if (result) {
    printf("%lu bins, all valid (total %d items)\n", dist.size(), sum);
  } else {
    printf("Invalid heap\n");
  }
}

tuple<bool, int> HeapPriorityDistributed::verify_all_with_size() {
  bool result = true;
  int sum = 0;
  
  for (auto it = dist.begin(); it != dist.end(); ++it) {
    auto this_bin = it->second.verify_all();
    result &= this_bin;
    sum += it->second.size();
    
    if (!this_bin) {
      printf("Invalid bin %d\n", it->first);
      it->second.debug_print();
    }
  }
  
  return tuple<bool, int>(result, sum);
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
  if (index == -1) { return Node<int>(); }
  
  bool dist_has_been_allocated = (dist.find(index) != dist.end());
  bool busy_has_been_allocated = (busy.find(index) != busy.end());
  
  if (!dist_has_been_allocated || !busy_has_been_allocated) {
    allocate_bin(index);
  }
  auto result = dist.at(index).peek();
  
  bool is_busy = true;
  while ((is_busy = busy.at(index).is_busy())) {}
  
  while(!(is_busy = busy.at(index).set_busy(is_busy))) {}
  dist.at(index).remove();
  while(!(is_busy = busy.at(index).set_free(is_busy))) {}
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
    
    if (element_value != -1 && element_value < min) {
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
  int snapshot_size = 0;
  for (auto it = dist.begin(); it != dist.end(); ++it) {
    s.insert(pair<int, Node<int>>(it->first, NULL));
    has_been_collected.insert(pair<int, bool>(it->first, false));
    snapshot_size++;
  }
  overlord_mutex.unlock();
  
  int free_count = 0;
  
  while (free_count < snapshot_size) {
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
        //s.at(thread_id) = dist_safe_access(thread_id).peek();
      }
    }
  }
  
  return s;
}

HeapPriorityBasic<int> HeapPriorityDistributed::dist_safe_access(int id) {
  while (true) {
    try {
      auto r = dist.at(id);
      return r;
    } catch (const out_of_range oor) {
      
    }
  }
}

AtomicWrapper HeapPriorityDistributed::busy_safe_access(int id) {
  while (true) {
    try {
      auto r = busy.at(id);
      return r;
    } catch (const out_of_range oor) {
      
    }
  }
}
