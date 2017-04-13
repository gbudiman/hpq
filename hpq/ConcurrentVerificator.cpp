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
  if (!verify_correctness) { return; }
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
  if (!verify_correctness) {
    printf("Skipped correctness verification. Run with -v option to enable verification");
    return;
  }
  
  FILE* pfile;
  pfile = fopen(HISTORY_FILE, "w");
  
  fprintf(pfile, "%s", s.c_str());
  fclose(pfile);
  
  verify_all();
}

int ConcurrentVerificator::find_in_oor(int cmp, int out) {
  auto cmp_index = find(oor_cmp.begin(), oor_cmp.end(), out);
  auto out_index = find(oor_out.begin(), oor_out.end(), cmp);
  int dist = 0;
  int result;
  
  if (cmp_index != oor_cmp.end() && out_index != oor_out.end()) {
    //int dist = abs(cmp_index - out_index);
    
    //printf("Out-of-order by %d ticks\n");
    oor_cmp.erase(cmp_index);
    oor_out.erase(out_index);
    
    dist = (int) max(distance(oor_cmp.begin(), cmp_index), distance(oor_out.begin(), out_index));
    result = -1;
  } else if (cmp_index != oor_cmp.end()) {
    oor_cmp.erase(cmp_index);
    oor_cmp.push_back(cmp);
    
    dist = (int) distance(cmp_index, oor_cmp.end());
    result = 0;
  } else if (out_index != oor_out.end()) {
    oor_out.erase(out_index);
    if (out != -1) { oor_out.push_back(out); }
    
    dist = (int) distance(out_index, oor_out.end());
    result = 0;
  } else {
    oor_cmp.push_back(cmp);
    oor_out.push_back(out);
    result = 1;
  }
  
  if (dist > max_oor_dist) {
    max_oor_dist = dist;
  }
  
  //dump_oor_content();
  return result;
}

void ConcurrentVerificator::dump_oor_content() {
  string s = "CMP {";
  string t = "OUT {";
  
  int max_index = (int) max(oor_cmp.size(), oor_out.size());
  for (int i = 0; i < max_index; i++) {
    if (i < oor_cmp.size()) { s += to_string(oor_cmp.at(i)) + " "; }
    if (i < oor_out.size()) { t += to_string(oor_out.at(i)) + " "; }
  }
  s += "}\n";
  t += "}\n";
  cout << s;
  cout << t;
}

void ConcurrentVerificator::verify_all() {
  FILE *pfile = NULL;
  pfile = fopen(HISTORY_FILE, "r");
  HeapPriorityBasic<int> h = HeapPriorityBasic<int>();
  int error_count = 0;
  int out_of_order_count = 0;
  oor_cmp = vector<int>();
  oor_out = vector<int>();
  max_oor_dist = 0;
  
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
          
          error_count += find_in_oor(cmp, out);
          //printf("[%d] Error in line %d, expected %d got %d\n", error_count, line_number, cmp, out);
          out_of_order_count++;
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
  
  if (out_of_order_count > 0) {
    printf("  Values received out-of-order: %d\n", out_of_order_count);
  }
  
  if (max_oor_dist > 0) {
    printf("  Maximum out-of-order distance: %d\n", max_oor_dist);
  }
}
