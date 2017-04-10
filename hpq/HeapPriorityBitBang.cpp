//
//  HeapPriorityBitBang.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/9/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "HeapPriorityBitBang.hpp"
#include "Lock.hpp"
using namespace std;

HeapPriorityBitBang::HeapPriorityBitBang() {
  initiallize_data();
}

void HeapPriorityBitBang::put(uint16_t priority, uint8_t thread_id) {
  while (1) {
    vector_push.lock();
      auto insert_position = data.size();
      data.push_back(NodeBitBang());
      auto new_node_data = data.at(insert_position).get();
    vector_push.unlock();
    
    long int new_value = NodeBitBang::wrap(priority, thread_id);
    if (data.at(insert_position).data.compare_exchange_strong(new_node_data, new_value)) {
      printf(">>> Thread %d: inserting %X to index %lu\n", thread_id, priority, insert_position);
      percolate((int) insert_position, thread_id);
      break;
    } else {
      printf("Thread collision, retrying...\n");
    }
    
  }
  
  //return *this;
}

void HeapPriorityBitBang::percolate(int i, uint8_t thread_id) {
  printf("    Thread %d percolating at index %d\n", thread_id, i);
  
  if (i == 1) {
    release(i, thread_id);
    return;
  }
  
  uint16_t p = get_parent_priority(i);
  uint16_t c = get_priority_at(i);
  
  if (p > c) {
    swap(i >> 1, i, thread_id);
  }
  
  while (!data.at(i).owner()) {
    printf("zzz Thread %d waiting for percolation at node %d\n", thread_id, i);
    this_thread::sleep_for(chrono::milliseconds(100));
  }
  release(i, thread_id);
  percolate(i >> 1, thread_id);
}

void HeapPriorityBitBang::swap(int a, int b, uint8_t thread_id) {
  bool parent_locked = false;
  bool child_locked = false;
  
  printf("<.> Thread %d attempting to lock %d and lock %d\n", thread_id, a, b);
  
  while (!parent_locked) {
    parent_locked = lock(a, thread_id);
    printf("<.> Thread %d can't lock parent %d, spinwaiting\n", thread_id, a);
    this_thread::sleep_for(chrono::milliseconds(100));
  }
  
  printf("</> Thread %d locked parent %d\n", thread_id, a);
  
  while (!child_locked) {
    child_locked = lock(b, thread_id);
    /*printf("<.> Thread %d can't lock child %d (owned by %d), spinwaiting\n", thread_id, b, data.at(b).owner());*/
    this_thread::sleep_for(chrono::milliseconds(100));
  }
  
  printf("<V> Thread %d locked parent %d and child %d\n", thread_id, a, b);
  
  auto data_a = data.at(a).data.load();
  auto data_b = data.at(b).data.load();
  bool success_a = false;
  bool success_b = false;
  
  if (data.at(a).data.compare_exchange_strong(data_a, data_b)) {
    success_a = true;
  }
  
  if (data.at(b).data.compare_exchange_strong(data_b, data_a)) {
    success_b = true;
  }
  
  if (success_a && success_b) {
    printf("<V> Thread %d swap successful\n", thread_id);
    release(a, thread_id);
    release(b, thread_id);
    printf("< > Thread %d released %d and released %d\n", thread_id, a, b);
  }
}

bool HeapPriorityBitBang::lock(int i, uint8_t thread_id) {
  return data.at(i).lock(thread_id);
}

void HeapPriorityBitBang::release(int i, uint8_t thread_id) {
  //printf("    Thread %d attempting to release node %d\n", thread_id, i);
  data.at(i).release(thread_id);
}

int HeapPriorityBitBang::size() {
  return data.size();
}

void HeapPriorityBitBang::initiallize_data() {
  data = vector<NodeBitBang>();
  data.push_back(NodeBitBang());
}

uint16_t HeapPriorityBitBang::get_parent_priority(int i) {
  return data.at(i >> 1).priority();
}

tuple<uint16_t, uint16_t> HeapPriorityBitBang::get_children_priority(int i) {
  int index_0 = i << 1;
  int index_1 = (i << 1) + 1;
  bool i0 = index_0 < data.size();
  bool i1 = index_1 < data.size();
  
  return make_tuple(i0 ? get_priority_at(index_0) : 0, i1 ? get_priority_at(index_1) : 0);
}

uint16_t HeapPriorityBitBang::take_priority() {
  return -1;
}

bool HeapPriorityBitBang::verify(int i) {
  bool is_correct = true;
  
  auto p = get_children_priority(i);
  int c = get_priority_at(i);
  int l = get<0>(p);
  int r = get<1>(p);
  
  if (l != 0) { is_correct &= (c <= l) && verify(i << 1); }
  if (r != 0) { is_correct &= (c <= r) && verify((i << 1) + 1); }
  
  return is_correct;
}

bool HeapPriorityBitBang::verify_all() {
  bool is_correct = true;
  is_correct &= verify(1);
  
  return is_correct;
}

void HeapPriorityBitBang::_verify_all() {
  if (verify_all()) {
    cout << "Valid min-heap";
  } else {
    //debug_print();
    cout << "Invalid min-heap";
  }
  
  cout << " (" << size() << " items)" << endl;
}

int HeapPriorityBitBang::get_height() {
  return ceil(log2(data.size()));
}

uint16_t HeapPriorityBitBang::get_priority_at(int i) {
  return i < data.size() ? data.at(i).priority() : -1;
}

void HeapPriorityBitBang::debug_print() {
  bool details = false;
  int digit_display = 6;
  int height = get_height();
  int leaf_count = pow(2, height - 1);
  int width = digit_display * leaf_count;
  
  printf("Tree height: %d (%lu items)\n", height, data.size() - 1);
  for (int i = 0; i < data.size(); i++) {
    if (details) printf("%d ", get_priority_at(i));
  }
  if (details) printf("\n");
  
  string s = "";
  int element_index = 1;
  for (int i = 0; i < height; i++) {
    int row_element_count = pow(2, i);
    float fpad = (float) (width - (row_element_count * digit_display)) / (float) (row_element_count + 1);
    int pad = round(fpad);
    float error = 0.0f;
    
    for (int k = 0; k < pad; k++) { s += " "; }
    if (details) printf("E:%d <h = %.1f> ", row_element_count, fpad);
    
    for (int j = 0; j < row_element_count; j++) {
      int rpad = pad;
      error += (fpad - pad);
      if (error > 1) {
        error -= 1.0f;
        rpad++;
      }
      char z[8];
      
      if (element_index < data.size()) {
        sprintf(z, " %4d ", get_priority_at(element_index++));//get<1>(data.at(element_index++)));
        s += z;
        //s += "______";
      } else {
        for (int k = 0; k < digit_display/2; k++) { s += " "; };
        s += "*";
        for (int k = 0; k < digit_display/2 - 1; k++) { s += " "; };
      }
      if (details) printf("<z = %d> ", digit_display);
      
      for (int k = 0; k < rpad; k++) { s += " "; }
      if (details) printf("<h = %d> ", rpad);
      
      
    }
    
    if (details) printf("\n");
    s += "\n\n";
  }
  
  cout << s << endl;
  
}
