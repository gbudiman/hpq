//
//  AtomicWrapper.hpp
//  hpq
//
//  Created by Gloria Budiman on 4/10/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#ifndef AtomicWrapper_hpp
#define AtomicWrapper_hpp

#include "main.hpp"

class AtomicWrapper {
public:
  AtomicWrapper();
  AtomicWrapper(const AtomicWrapper& other);
  bool is_busy();
  bool set_busy();
  bool set_free();
  
private:
  std::atomic_bool val;
};
#endif /* AtomicWrapper_hpp */
