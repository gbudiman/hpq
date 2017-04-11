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

HeapPriorityDistributed2::HeapPriorityDistributed2(const HeapPriorityDistributed2& other) {
  
}

void HeapPriorityDistributed2::initialize(int bins) {
  dist = vector<HeapPriorityBasic<int>>();
  minbin = vector<AtomicMin>();
  mutexes = vector<mutex>(bins);
  
  for (int i = 0; i < bins; i++) {
    dist.push_back(HeapPriorityBasic<int>());
    minbin.push_back(AtomicMin());
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
  int index = get_minbin_index();
  
  if (index == -1) {
    return Node<int>();
  }
  
  mutexes.at(index).lock();
  Node<int> out = dist.at(index).take();
  auto new_min = dist.at(index).peek_priority();
  if (new_min == -1) { new_min = __INT_MAX__; }
  mutexes.at(index).unlock();
  
  update_minbin(index, new_min);
  
  return out;
}

int HeapPriorityDistributed2::get_minbin_index() {
  int min = __INT_MAX__;
  int index = -1;
  
  string s = "";
  for (int i = 0; i < minbin.size(); i++) {
    int val = minbin.at(i).get();
    s += to_string(val) + " ";
    
    if (val < min) {
      min = val;
      index = i;
    }
  }
  
  //printf("   Min at index %3d %s\n", index, s.c_str());
  return index;
}

void HeapPriorityDistributed2::update_minbin(int index, int new_val) {
  minbin.at(index).set(new_val);
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
