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
#include <algorithm>
#include <atomic>
#include <unordered_map>

#define DEFAULT_WORKLOAD_THREADS 4
#define LOAD_ITERATION_LIMIT 8
#define LOAD_DELAYED_START 100000 //microseconds
#define LOAD_PRIORITY_MAX 500
#define LOAD_PRIORITY_MIN 1
#define LOAD_PUT_TO_TAKE_RATIO 0.5f
#define LOAD_SLEEP 0 // microseconds
#define SHOW_STEPS 0
#define DO_VALIDATE 1
#define SHOW_PROGRESS 0

#define NON_VIOLATING 0
#define UNCONDITIONAL_RELEASE 1

#define DEFAULT_NODE 0xFFFFFFFF00000000
#define BITBANG_INVALID 0xFFFFFFFFFFFFFFFF

#define OP_TAKE 0
#define OP_PUT 1

#endif /* main_h */
