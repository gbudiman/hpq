//
//  HeapPriorityBasic.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/5/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "HeapPriorityBasic.hpp"
using namespace std;

template <class T>
HeapPriorityBasic<T>::HeapPriorityBasic() {
  initialize_data();
}

template <class T>
HeapPriorityBasic<T>::HeapPriorityBasic(vector<int> inits) {
  load(inits);
}

template <class T>
HeapPriorityBasic<T> HeapPriorityBasic<T>::load(vector<int> inits) {
  initialize_data();
  
  for (int i = 0; i < inits.size(); i++) {
    data.push_back(Node<T>(inits.at(i)));
  }
  
  return *this;
}

template <class T>
bool HeapPriorityBasic<T>::identical_contents(vector<int> cmps) {
  int sum_cmps = 0;
  int sum_heap = 0;
  
  for (int i = 0; i < cmps.size(); i++) {
    if (cmps.at(i) == -1) continue;
    sum_cmps += cmps.at(i);
  }
  
  for (int i = 1; i < data.size(); i++) {
    int p = get_priority_at(i);
    if (p == -1) continue;
    sum_heap += p;
  }
  
  return sum_cmps == sum_heap;
}

template <class T>
bool HeapPriorityBasic<T>::weak_equals(vector<int> cmps) {
  // Weak equals requires the following to be true:
  // - the root element to be equal to the first element in cmps
  // - the heap structure to be valid
  // - the length is equal
  // - the heap contains identical data but not in any particular order
  
  
  bool correct_root = peek_priority() == cmps.at(0);
  
  
  return verify_all() && identical_contents(cmps) && correct_root;
}

template <class T>
bool HeapPriorityBasic<T>::equals(vector<int> cmps) {
  bool result = true;
  
  if (data.size() - 1 != cmps.size()) { return false; }
  
  for (int i = 1; i < data.size(); i++) {
    result &= get_priority_at(i) == cmps.at(i - 1);
  }
  
  return result;
}

template <class T>
void HeapPriorityBasic<T>::initialize_data() {
  data = vector<Node<T>>();
  data.push_back(Node<T>());
}

template <class T>
int HeapPriorityBasic<T>::size() {
  return data.size();
}

template <class T>
tuple<int, int> HeapPriorityBasic<T>::get_children_priority(int i) {
  int index_0 = i << 1;
  int index_1 = (i << 1) + 1;
  bool i0 = index_0 < data.size();
  bool i1 = index_1 < data.size();
  
  return make_tuple(i0 ? get_priority_at(index_0) : -1, i1 ? get_priority_at(index_1) : -1);
}

template <class T>
void HeapPriorityBasic<T>::_get_children_priority(int i) {
  tuple<int, int> p = get_children_priority(i);
  int a = get<0>(p);
  int b = get<1>(p);
  
  string s = "Children of " + to_string(get_priority_at(i)) + " is: ";
  s += (a == -1) ? " <DNE> " : to_string(a) + ", ";
  s += (b == -1) ? " <DNE> " : to_string(b);
  
  cout << s << endl;
}

template <class T>
int HeapPriorityBasic<T>::get_parent_priority(int i) {
  return i < data.size() ? get_priority_at(i >> 1) : -1;
}

template <class T>
void HeapPriorityBasic<T>::_get_parent_priority(int i) {
  string s = "Parent of " + to_string(get_priority_at(i)) + " is: " + to_string(get_parent_priority(i));
  
  cout << s << endl;
}

template <class T>
int HeapPriorityBasic<T>::get_priority_at(int i) {
  return i < data.size() ? data.at(i).priority : -1;

}

template <class T>
HeapPriorityBasic<T> HeapPriorityBasic<T>::put(int i) {
  data.push_back(Node<T>(i));
  percolate((int) data.size() - 1);
  
  if (SHOW_STEPS) { cout << " -> " << i << endl; _verify_all(); debug_print(); }
  return *this;
}

template <class T>
void HeapPriorityBasic<T>::percolate(int i) {
  int p = get_parent_priority(i);
  int c = get_priority_at(i);
  
  if (p > c) {
    swap(i, i >> 1);
    percolate(i >> 1);
  }
}

template <class T>
int HeapPriorityBasic<T>::take_priority() {
  return take().priority;
}

template <class T>
int HeapPriorityBasic<T>::peek_priority() {
  return peek().priority;
}

template <class T>
HeapPriorityBasic<T> HeapPriorityBasic<T>::remove() {
  if (!is_empty()) {
    apply_removal();
  }
  
  return *this;
}

template <class T>
Node<T> HeapPriorityBasic<T>::peek() {
  if (is_empty()) { return Node<T>(); }
  return data.at(1);
}

template <class T>
Node<T> HeapPriorityBasic<T>::take() {
  if (is_empty()) { return Node<T>(); }
  return apply_removal();
}

template <class T>
void HeapPriorityBasic<T>::last_to_first() {
  data.at(1) = data.at(data.size() - 1);
  data.pop_back();
}

template <class T>
bool HeapPriorityBasic<T>::is_empty() {
  return data.size() == 1;
}

template <class T>
Node<T> HeapPriorityBasic<T>::apply_removal() {
  auto out = data.at(1);
  last_to_first();
  sift(1);
  
  if (SHOW_STEPS) { cout << " <- " << out.priority << endl; _verify_all(); debug_print(); }
  return out;
}

template <class T>
void HeapPriorityBasic<T>::sift(int i) {
  auto ch = get_children_priority(i);
  int c = get_priority_at(i);
  int a = get<0>(ch);
  int b = get<1>(ch);
  
  if (a != -1 && b != -1) {
    int min = std::min(a, b);
    if (c > min && a == min) {
      swap(i, i << 1);
      sift(i << 1);
    } else if (c > min && b == min) {
      swap(i, (i << 1) + 1);
      sift((i << 1) + 1);
    }
  } else if (a == -1 && b != -1) {
    if (c > b) {
      swap(i, (i << 1) + 1);
      sift((i << 1) + 1);
    }
  } else if (a != -1 && b == -1) {
    if (c > a) {
      swap(i, i << 1);
      sift(i << 1);
    }
  }
}

template <class T>
void HeapPriorityBasic<T>::swap(int a, int b) {
  auto x = data.at(a);
  data.at(a) = data.at(b);
  data.at(b) = x;
}

template <class T>
void HeapPriorityBasic<T>::_verify_all() {
  if (verify_all()) {
    cout << "Valid min-heap" << endl;
  } else {
    debug_print();
    cout << "Invalid min-heap" << endl;
  }
}

template <class T>
bool HeapPriorityBasic<T>::verify_all() {
  bool is_correct = true;
  is_correct &= verify(1);
  
  return is_correct;
}

template <class T>
bool HeapPriorityBasic<T>::verify_all(vector<int> cmps) {
  return verify(1) && equals(cmps);
}

template <class T>
bool HeapPriorityBasic<T>::verify(int i) {
  bool is_correct = true;
  
  auto p = get_children_priority(i);
  int c = get_priority_at(i);
  int l = get<0>(p);
  int r = get<1>(p);
  
  if (l != -1) { is_correct &= (c <= l) && verify(i << 1); }
  if (r != -1) { is_correct &= (c <= r) && verify((i << 1) + 1); }
  
  return is_correct;
}

template <class T>
void HeapPriorityBasic<T>::debug_print() {
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

template <class T>
int HeapPriorityBasic<T>::get_height() {
  return ceil(log2(data.size()));
}

template class HeapPriorityBasic<char>;
template class HeapPriorityBasic<int>;
