//
//  ConcurrentVerificator.hpp
//  hpq
//
//  Created by Gloria Budiman on 4/11/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#ifndef ConcurrentVerificator_hpp
#define ConcurrentVerificator_hpp
#define HISTORY_FILE "history.txt"

#include "main.hpp"
#include "HeapPriorityBasic.hpp"

class ConcurrentVerificator {
public:
  ConcurrentVerificator();
  ConcurrentVerificator(const ConcurrentVerificator& other);
  ConcurrentVerificator operator=(const ConcurrentVerificator& other);
  void record(int op, int val, int id);
  void done();
  
private:
  void verify_all();
  int find_in_oor(int cmp, int out);
  void dump_oor_content();
  int get_max_oor_content_length();
  
  char val_buffer[8];
  char id_buffer[8];
  std::string s;
  
  std::mutex m;
  
  std::vector<int> oor_cmp;
  std::vector<int> oor_out;
  int max_oor_dist;
};

#endif /* ConcurrentVerificator_hpp */
