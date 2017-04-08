//
//  main.hpp
//  hpq
//
//  Created by Gloria Budiman on 4/5/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#ifndef main_h
#define main_h

#include <iostream>
#include <vector>
#include <thread>
#include <tuple>
#include <math.h>
#include <string>
#include <mutex>

#define DEFAULT_WORKLOAD_THREADS 32
#define LOAD_ITERATION_LIMIT 1000
#define LOAD_DELAYED_START 100000 //microseconds
#define LOAD_PRIORITY_MAX 500
#define LOAD_PRIORITY_MIN 1
#define LOAD_PUT_TO_TAKE_RATIO 0.75f
#define LOAD_SLEEP 0 // microseconds
#define SHOW_STEPS 0
#define DO_VALIDATE 1
#define SHOW_PROGRESS 0

#endif /* main_h */
