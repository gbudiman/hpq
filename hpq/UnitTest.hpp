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

#define PUT_TO_TAKE_RATIO 0.5f
#define INTEGRATION_OPS 50000
#define PRIORITY_MAX 500
#define PRIORITY_MIN 1
#define INTEGRATION_ITERATION 1000

class UnitTest {
public:
  UnitTest();
  
private:
  void test_heap_structure();
  void test_take();
  void test_put();
  void manual_integration();
  bool integration_test();
  void stress_test();
  
  void evaluate(bool b, std::string s);
  void evaluate_content(bool b);
  
  int random_priority();
  int random_operation();
};


#endif /* UnitTest_hpp */
