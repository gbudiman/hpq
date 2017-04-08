//
//  Node.hpp
//  hpq
//
//  Created by Gloria Budiman on 4/7/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#include "main.hpp"

template <class T>
class Node {
public:
  Node();
  Node(int i);
  Node(T item, int i);
  
  T item;
  int priority;
  
private:
};
#endif /* Node_hpp */
