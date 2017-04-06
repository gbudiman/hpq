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
  
  data.push_back(std::make_tuple('X', -1));
  for (int i = 0; i < inits.size(); i++) {
    auto t = std::make_tuple('X', inits.at(i));
    data.push_back(t);
  }
  
  return *this;
}

template <class T>
void HeapPriorityBasic<T>::initialize_data() {
  data = std::vector<std::tuple<T, int>>();
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
    if (details) printf("%d ", get<1>(data.at(i)));
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
        sprintf(z, " %4d ", get<1>(data.at(element_index++)));
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
