//
//  Workload.hpp
//  hpq
//
//  Created by Gloria Budiman on 4/5/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#ifndef Workload_hpp
#define Workload_hpp

#include "main.hpp"

class Workload {
public:
  Workload();
  Workload(int n);
  static void run(int id);
  
private:
  void initialize_workload(int n);
  std::vector<std::thread> loaders;
};

#endif /* Workload_hpp */
