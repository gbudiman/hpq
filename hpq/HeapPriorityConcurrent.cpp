//
//  HeapPriorityConcurrent.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/7/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "HeapPriorityConcurrent.hpp"
#include "Lock.hpp"

using namespace std;

template <class T>
HeapPriorityConcurrent<T> HeapPriorityConcurrent<T>::cput(int i) {
  vector_push.lock();
  
  this->data.push_back(Node<T>(i));
  vector_push.unlock();
  
  this->percolate((int) this->data.size() - 1);
  
  if (SHOW_STEPS) { cout << " -> " << i << endl; this->_verify_all(); this->debug_print(); }
  return *this;
}

template class HeapPriorityConcurrent<char>;
template class HeapPriorityConcurrent<int>;
