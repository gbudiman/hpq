//
//  ConcurrentVerificator.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/11/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "ConcurrentVerificator.hpp"
using namespace std;

ConcurrentVerificator::ConcurrentVerificator() {
  string s = "";
}

ConcurrentVerificator::ConcurrentVerificator(const ConcurrentVerificator& other) {
  
}

ConcurrentVerificator ConcurrentVerificator::operator=(const ConcurrentVerificator &other) {
  return *this;
}

void ConcurrentVerificator::record(int op, int val, int id) {
  m.lock();
  sprintf(val_buffer, "%3d ", val);
  sprintf(id_buffer, "[%3d]", id);
  
  s += (op == OP_PUT ? "        " : ">> ")
     + string(val_buffer)
     + (op == OP_TAKE ? "     " : "")
     + string(id_buffer)
     + "\n";
  m.unlock();
}

void ConcurrentVerificator::done() {
  FILE* pfile;
  pfile = fopen(HISTORY_FILE, "w");
  
  fprintf(pfile, "%s", s.c_str());
  fclose(pfile);
  
  verify_all();
}

void ConcurrentVerificator::verify_all() {
  FILE *pfile = NULL;
  pfile = fopen(HISTORY_FILE, "r");
  HeapPriorityBasic<int> h = HeapPriorityBasic<int>();
  int error_count = 0;
  
  if (pfile != NULL) {
    int line_number = 0;
    char line[128];
    while (fgets(line, sizeof(line), pfile) != NULL) {
      line_number++;
      if (line[0] == '>') {
        int out;
        if (line[4] == '-') {
          out = -1;
        } else {
          sscanf(line, ">> %d", &out);
        }

        int cmp = h.take_priority();
        
        if (out != cmp) {
          printf("Error in line %d, expected %d got %d\n", line_number, cmp, out);
          error_count++;
        }
      } else {
        int val;
        sscanf(line, " %d", &val);
        h.put(val);
      }
    }
    
    fclose(pfile);
  } else {
    perror("history.txt");
  }
  
  if (error_count == 0) {
    printf("Run correctness test passed\n");
  }
}
