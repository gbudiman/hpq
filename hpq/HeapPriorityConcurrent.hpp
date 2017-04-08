//
//  HeapPriorityConcurrent.hpp
//  hpq
//
//  Created by Gloria Budiman on 4/7/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#ifndef HeapPriorityConcurrent_hpp
#define HeapPriorityConcurrent_hpp

#define NODE_OK 0
#define NODE_BUSY 1

#include "main.hpp"
#include "HeapPriorityBasic.hpp"

template <class T>
class HeapPriorityConcurrent : public HeapPriorityBasic<T> {
public:
  HeapPriorityConcurrent<T> cput(int i);
};

#endif /* HeapPriorityConcurrent_hpp */
