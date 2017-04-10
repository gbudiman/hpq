//
//  AtomicWrapper.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/10/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "AtomicWrapper.hpp"

AtomicWrapper::AtomicWrapper() {
  val.store(false);
}

AtomicWrapper::AtomicWrapper(const AtomicWrapper& other) {
  val.store(false);
}

bool AtomicWrapper::is_busy() {
  return val.load();
}

bool AtomicWrapper::set_busy(bool expectation) {
  return val.compare_exchange_strong(expectation, true);
}

bool AtomicWrapper::set_free(bool expectation) {
  return val.compare_exchange_strong(expectation, false);
}
