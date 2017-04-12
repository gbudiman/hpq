//
//  HeapPriorityDistributed2.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/11/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "HeapPriorityDistributed2.hpp"
using namespace std;

HeapPriorityDistributed2::HeapPriorityDistributed2() {
  initialize(DEFAULT_WORKLOAD_THREADS);
}

HeapPriorityDistributed2::HeapPriorityDistributed2(int bins) {
  initialize(bins);
}

HeapPriorityDistributed2::HeapPriorityDistributed2(const HeapPriorityDistributed2& other) {
  
}

void HeapPriorityDistributed2::initialize(int bins) {
  dist = vector<HeapPriorityBasic<int>>();
  minbin = vector<int>();
  global_minbin = -1;
  mutexes = vector<mutex>(bins);
  
  for (int i = 0; i < bins; i++) {
    dist.push_back(HeapPriorityBasic<int>());
    minbin.push_back(__INT_MAX__);
  }
}

HeapPriorityDistributed2 HeapPriorityDistributed2::put(int thread_id, int priority) {
  mutexes.at(thread_id).lock();
  auto new_min = dist.at(thread_id).put(priority).peek_priority();
  mutexes.at(thread_id).unlock();
  
  update_minbin(thread_id, new_min);
  return *this;
}

int HeapPriorityDistributed2::take_priority() {
  return take().priority;
}

Node<int> HeapPriorityDistributed2::take() {
  int index = global_minbin;
  
  if (index != -1) {
    mutexes.at(index).lock();
    
    Node<int> out = dist.at(index).take();
    auto new_min = dist.at(index).peek_priority();
    
    mutexes.at(index).unlock();
    
    update_minbin(index, new_min);
    return out;
  }
  
  return Node<int>();
}

void HeapPriorityDistributed2::update_minbin(int index, int new_val) {
  int old_val = minbin.at(index);
  if (new_val < old_val) {
    minbin.at(index) = (new_val == -1 ? __INT_MAX__ : new_val);
    update_global_minbin();
  }
}

void HeapPriorityDistributed2::update_global_minbin() {
  int min = __INT_MAX__;
  int index = -1;
  minbin_mutex.lock();
  
  for (int i = 0; i < minbin.size(); i++) {
    auto local_min = minbin.at(i);
    //if (local_min == -1) { continue; }
    if (local_min < min) {
      min = local_min;
      index = i;
    }
  }
  
  global_minbin = index;
  
  /*debug_print_mutex.lock();
  string s = "";
  for (int i = 0; i < minbin.size(); i++) {
    auto d = minbin.at(i);
    if (d == __INT_MAX__) {
      s += "X ";
    } else {
      s += to_string(minbin.at(i)) + " ";
    }
  }
  
  printf("|| %s [%d]\n", s.c_str(), global_minbin);
  debug_print_mutex.unlock();*/
  
  minbin_mutex.unlock();
}

void HeapPriorityDistributed2::_verify_all() {
  if (verify_all()) {
    printf("All bins are valid\n");
  }
}

bool HeapPriorityDistributed2::verify_all() {
  bool correct = true;
  int sum = 0;
  
  for (int i = 0; i < dist.size(); i++) {
    bool sub_correct = verify_bin(i);
    correct &= sub_correct;
    sum += dist.at(i).size();
    
    if (!sub_correct) {
      printf("Invalid bin %d\n", i);
    }
  }
  
  printf("%d items verified\n", sum);
  return correct;
}

bool HeapPriorityDistributed2::verify_bin(int index) {
  return dist.at(index).verify_all();
}
