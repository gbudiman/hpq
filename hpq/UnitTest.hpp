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
#include "HeapPriorityDistributed2.hpp"
#include "RunnerThread.hpp"

#define PUT_TO_TAKE_RATIO 0.5f
#define INTEGRATION_OPS 10000
#define PRIORITY_MAX 500
#define PRIORITY_MIN 1
#define INTEGRATION_ITERATION 10000

class UnitTest {
public:
  UnitTest();
  
private:
  void test_heap_structure();
  void test_take();
  void test_put();
  void test_concurrent_put();
  void manual_integration();
  bool integration_test();
  void stress_test();
  
  void evaluate(bool b, std::string s);
  void evaluate_content(bool b);
  
  void test_distributed();
  void distributed_runner(std::shared_ptr<HeapPriorityDistributed2> hd, std::vector<int> v, uint8_t thread_id);
  void distributed_taker(std::shared_ptr<HeapPriorityDistributed2> hd, uint8_t);
};


#endif /* UnitTest_hpp */
