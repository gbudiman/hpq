//
//  UnitTest.hpp
//  hpq
//
//  Created by Gloria Budiman on 4/5/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#ifndef UnitTest_hpp
#define UnitTest_hpp

#include "main.hpp"
#include "HeapPriorityBasic.hpp"

class UnitTest {
public:
  UnitTest();
  
private:
  void test_heap_structure();
  void test_take();
  void test_put();
};


#endif /* UnitTest_hpp */
