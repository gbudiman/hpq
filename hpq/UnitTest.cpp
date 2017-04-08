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
  srand(time(NULL));
  /*test_heap_structure();
  test_take();
  test_put();
  
  test_take();*/
  
  //manual_integration();
  //integration_test();
  //stress_test();
  //threaded_stress_test();
}

void UnitTest::test_heap_structure() {
  cout << "======= Begin heap structure test" << endl;
  vector<int> v0 = {1,3,5,12,16,8,10,13,14,17,19,12,15,11,13};
  vector<int> v1 = {1,3,5,12,16,8,10,13,14,17,19,12,15};
  vector<int> v2 = {1,3,5,12,16,8,10,13,14,17,19,12,15,11,13,18,16,14,12,13};
  vector<int> v3 = {1,3,5,22};
  vector<int> v4 = {};
  vector<int> v5 = {2};
  HeapPriorityBasic<int> h = HeapPriorityBasic<int>(v0);
  evaluate(h.equals(v0),
           "Structure instantiation");
  evaluate(h.get_children_priority(3) == make_tuple(8, 10),
           "Get children priority");
  evaluate(h.get_children_priority(12) == make_tuple(-1, -1),
           "Get non-existant children priority");
  evaluate(h.get_children_priority(1) == make_tuple(3, 5),
           "Get children of root node");
  evaluate(h.get_parent_priority(8) == 12,
           "Get parent priority");
  evaluate(h.get_parent_priority(20) == -1,
           "Get parent priority of invalid node");
  evaluate(h.verify_all(v0),
           "Valid min-heap structure and content");
  
  evaluate_content(h.load(v1).verify_all(v1));
  evaluate_content(!h.load(v2).verify_all(v2));
  evaluate_content(h.load(v3).verify_all(v3));
  evaluate_content(h.load(v4).verify_all(v4));
  evaluate_content(h.load(v5).verify_all(v5));
}

void UnitTest::test_take() {
  cout << "======= Begin take() operation test" << endl;
  vector<int> v0 = {1,3,5,6,4,8};
  vector<int> v1 = {17,18,22};
  HeapPriorityBasic<int> h = HeapPriorityBasic<int>();
  evaluate_content(h.load(v0).verify_all(v0));
  
  evaluate(h.take_priority() == 1, "First take() operation");
  evaluate(h.take_priority() == 3, "Follow-up take()");
  evaluate(h.take_priority() == 4, "Follow-up take()");
  evaluate(h.take_priority() == 5, "Follow-up take()");
  evaluate(h.take_priority() == 6, "Follow-up take()");
  evaluate(h.take_priority() == 8, "Follow-up take()");
  evaluate(h.take_priority() == -1, "take() on empty sturcture");
  evaluate_content(h.verify_all(vector<int>()));
  
  evaluate_content(h.load(v1).verify_all(v1));
  evaluate_content(h.remove().verify_all());
  evaluate_content(h.remove().verify_all());
  evaluate_content(h.remove().verify_all());
  evaluate_content(h.remove().verify_all());
}

void UnitTest::test_put() {
  cout << "======= Begin put() operation test" << endl;
  vector<int> v0 = {1,3,5,6,4,8};
  vector<int> v1 = {1,2,3,4,5,6,8};
  vector<int> v2 = {1,2,3,3,4,5,6,8};
  vector<int> v3 = {1,2,3,3,4,5,5,6,8};
  vector<int> v4 = {1,2,3,3,4,5,5,6,8,15};
  HeapPriorityBasic<int> h = HeapPriorityBasic<int>(v0);
  evaluate_content(h.load(v0).verify_all(v0));
  
  evaluate(h.put(2).weak_equals(v1), "First put() operation");
  evaluate(h.put(3).weak_equals(v2), "Follow-up put() operation");
  evaluate(h.put(5).weak_equals(v3), "Follow-up put() operation");
  evaluate(h.put(15).weak_equals(v4), "Follow-up put() operation");
}

void UnitTest::manual_integration() {
  HeapPriorityBasic<int> h = HeapPriorityBasic<int>();
  
  h.put(308);
  h.put(394);
  h.put(418);
  h.put(126);
  h.put(342);
  h.put(209);
  h.take();
  h.put(109);
  h.put(204);
  h.put(452);
  h.put(276);
  h.take();
  h.put(390);
  h.put(274);
  h.put(287);
  h.take();
  h.put(161);
  h.take();
  h.put(493);
  
}

bool UnitTest::integration_test() {
  //cout << "======= Begin integration test with " << INTEGRATION_OPS << " operations, " << PUT_TO_TAKE_RATIO << " put/take ratio" << endl;
  vector<int> puts = vector<int>();
  vector<int> takes = vector<int>();
  vector<int> diff = vector<int>();
  string s = "";
  HeapPriorityBasic<int> h = HeapPriorityBasic<int>();
  
  for (int i = 0; i < INTEGRATION_OPS; i++) {
    int operation = Workload::random_operation();
    int priority;
    int out;
    
    switch(operation) {
      case 0:
        priority = Workload::random_priority();
        h.put(priority);
        puts.push_back(priority);
        s += " -> " + to_string(priority) + "\n";
        break;
      case 1:
        out = h.take_priority();
        takes.push_back(out);
        s += " <- " + to_string(out) + "\n";
        break;
    }
  }
  
  sort(puts.begin(), puts.end());
  sort(takes.begin(), takes.end());
  set_difference(puts.begin(), puts.end(), takes.begin(), takes.end(), inserter(diff, diff.begin()));
  
  return h.verify_all() && h.identical_contents(diff);
}

void UnitTest::stress_test() {
  cout << "======= Begin stress test (" << INTEGRATION_ITERATION << " iterations) with " << INTEGRATION_OPS << " operations, " << PUT_TO_TAKE_RATIO << " put/take ratio..." << endl;
  bool pass = true;
  for (int i = 0; i < INTEGRATION_ITERATION; i++) {
    pass &= integration_test();
    if (!pass) { break; }
    if (i % (INTEGRATION_ITERATION / 20) == 0) {
      printf("%3.0f%% completed\n", (float) i / INTEGRATION_ITERATION * 100);
    }
  }
  
  cout << "Stress test " << (pass ? "PASSED" : "FAILED") << endl;
}

void UnitTest::evaluate(bool b, string s) {
  cout << "Test " << (b ? "passed" : "FAILED") << ": " << s << endl;
}

void UnitTest::evaluate_content(bool b) {
  if (b) {
    cout << "Test passed: Valid min-heap structure and content" << endl;
  } else {
    cout << "Test FAILED: Invalid min-heap structure and/or content" << endl;
  }
}
