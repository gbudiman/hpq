//
//  AtomicMin.hpp
//  hpq
//
//  Created by Gloria Budiman on 4/11/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#ifndef AtomicMin_hpp
#define AtomicMin_hpp

#include "main.hpp"

class AtomicMin {
public:
  AtomicMin();
  AtomicMin(const AtomicMin& other);
  int get();
  bool set(int new_val);
  
  std::atomic<int> data;
};

#endif /* AtomicMin_hpp */
