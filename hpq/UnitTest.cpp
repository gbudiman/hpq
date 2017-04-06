//
//  UnitTest.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/5/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "UnitTest.hpp"
using namespace std;

UnitTest::UnitTest() {
  
  
  test_heap_structure();
}

void UnitTest::test_heap_structure() {
  vector<int> v0 = {1,3,5,12,16,8,10,13,14,17,19,12,15,11,13};
  vector<int> v1 = {1,3,5,12,16,8,10,13,14,17,19,12,15};
  vector<int> v2 = {1,3,5,12,16,8,10,13,14,17,19,12,15,11,13,18,16,14,12,13};
  vector<int> v3 = {1,3,5,12};
  HeapPriorityBasic<int> h = HeapPriorityBasic<int>(v0);
  h.debug_print();
  h._get_children_priority(3);
  h._get_children_priority(12);
  h._get_children_priority(1);
  h._get_parent_priority(8);
  
  h.load(v1).debug_print();
  h.load(v2).debug_print();
  h.load(v3).debug_print();
}
