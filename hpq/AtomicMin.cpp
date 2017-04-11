//
//  AtomicMin.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/11/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "AtomicMin.hpp"

AtomicMin::AtomicMin() {
  data.store(__INT_MAX__);
}

AtomicMin::AtomicMin(const AtomicMin& other) {
  data.store(__INT_MAX__);
}

bool AtomicMin::set(int new_val) {
  while(true) {
    int old_val = data.load();
    
    if (data.compare_exchange_strong(old_val, new_val)) {
      return true;
    }
  }
}

int AtomicMin::get() {
  return data.load();
}
