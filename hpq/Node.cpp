//
//  Node.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/7/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "Node.hpp"
using namespace std;

template <class T>
Node<T>::Node() {
  item = NULL;
  priority = -1;
}

template <class T>
Node<T>::Node(int i) {
  item = NULL;
  priority = i;
}

template <class T>
Node<T>::Node(T item, int i) {
  this->item = item;
  priority = i;
}

template class Node<char>;
template class Node<int>;
