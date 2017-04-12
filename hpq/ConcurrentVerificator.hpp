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
  void record(int op, int val, int id);
  void done();
  
private:
  void verify_all();
  
  char val_buffer[8];
  char id_buffer[8];
  std::string s;
  
  std::mutex m;
};

#endif /* ConcurrentVerificator_hpp */
