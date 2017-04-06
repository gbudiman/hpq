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
  //test_heap_structure();
  //test_take();
  test_put();
}

void UnitTest::test_heap_structure() {
  vector<int> v0 = {1,3,5,12,16,8,10,13,14,17,19,12,15,11,13};
  vector<int> v1 = {1,3,5,12,16,8,10,13,14,17,19,12,15};
  vector<int> v2 = {1,3,5,12,16,8,10,13,14,17,19,12,15,11,13,18,16,14,12,13};
  vector<int> v3 = {1,3,5,22};
  vector<int> v4 = {};
  vector<int> v5 = {2};
  HeapPriorityBasic<int> h = HeapPriorityBasic<int>(v0);
  h.debug_print();
  h._get_children_priority(3);
  h._get_children_priority(12);
  h._get_children_priority(1);
  h._get_parent_priority(8);
  h._verify_all();
  
  h.load(v1).debug_print();
  h._verify_all();
  h.load(v2).debug_print();
  h._verify_all();
  h.load(v3).debug_print();
  h._verify_all();
  h.load(v4)._verify_all();
  h.load(v5)._verify_all();
}

void UnitTest::test_take() {
  vector<int> v0 = {1,3,5,6,4,8};
  HeapPriorityBasic<int> h = HeapPriorityBasic<int>(v0);
  h._verify_all();
  h.debug_print();
  h.take();
  h._verify_all();
  h.debug_print();
  h.take();
  h._verify_all();
  h.debug_print();
  h.take();
  h._verify_all();
  h.debug_print();
  h.take();
  h._verify_all();
  h.debug_print();
  h.take();
  h._verify_all();
  h.debug_print();
  h.take();
  h._verify_all();
  h.debug_print();
}

void UnitTest::test_put() {
  vector<int> v0 = {1,3,5,6,4,8};
  HeapPriorityBasic<int> h = HeapPriorityBasic<int>(v0);
  h._verify_all();
  h.debug_print();
  h.put(2);
  h._verify_all();
  h.debug_print();
  h.put(3);
  h._verify_all();
  h.debug_print();
  h.put(5);
  h._verify_all();
  h.debug_print();
}
