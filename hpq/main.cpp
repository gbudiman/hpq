//
//  main.cpp
//  hpq
//
//  Created by Gloria Budiman on 4/5/17.
//  Copyright © 2017 gbudiman. All rights reserved.
//

#include "main.hpp"
#include "Workload.hpp"
#include "UnitTest.hpp"
#include "HeapPriorityBasic.hpp"
#include "HeapPriorityDistributed2.hpp"
#include "ConcurrentVerificator.hpp"
using namespace std;

int num_threads = DEFAULT_WORKLOAD_THREADS;
int iteration_limit = LOAD_ITERATION_LIMIT;
int load_sleep = LOAD_SLEEP;
float load_ratio = LOAD_PUT_TO_TAKE_RATIO;
bool verify_correctness = DO_VALIDATE;
bool immediately_terminate = false;
bool hide_progress = false;
int prepopulate = 0;

void print_help() {
  cout << "Configurable run parameters:\n";
  cout << " -h Show this help\n\n";
  cout << " -d Sleep between iteration (microseconds) (default: " << LOAD_SLEEP << ")\n";
  cout << " -i Iteration per thread                   (default: " << LOAD_ITERATION_LIMIT << ")\n";
  cout << " -p Prepopulate with items                 (default: " << prepopulate << ")\n";
  cout << " -r put() to take() percentage ratio       (default: " << (int) (LOAD_PUT_TO_TAKE_RATIO * 100) << ")\n";
  cout << " -t Number of threads                      (default: " << DEFAULT_WORKLOAD_THREADS << ")\n";
  cout << " -v Verify correctness                     (default: FALSE)\n";
  cout << " -x Hide per-thread progress\n";
  cout << endl;

  immediately_terminate = true;
}

void process_args(int argc, char* argv[]) {
  char opt;
  
  while ((opt = getopt(argc, argv, "hvxt:i:r:d:p:")) != -1) {
    switch(opt) {
      case 'h': print_help(); break;
      case 'v': verify_correctness = true; break;
      case 't': num_threads = atoi(optarg); break;
      case 'i': iteration_limit = atoi(optarg); break;
      case 'r': load_ratio = (float) (atoi(optarg)) / 100.0f; break;
      case 'x': hide_progress = true; break;
      case 'd': load_sleep = atoi(optarg); break;
      case 'p': prepopulate = atoi(optarg); break;
    }
  }
}

void print_parameters() {
  printf("Sleep between iteration:  %6d ms\n", load_sleep);
  printf("Put to take ratio:        %6.0f %%\n", load_ratio * 100);
  printf("Iterations count:         %6d\n", iteration_limit);
  printf("Prepopulate:              %6d\n", prepopulate);
  printf("Verify correctness:        %s\n", verify_correctness ? " TRUE" : "FALSE");
  printf("Show progress:             %s\n", hide_progress ? "FALSE" : " TRUE");
  printf("\n");
}

int main(int argc, char * argv[]) {
  process_args(argc, argv);
  if (immediately_terminate) { return 0; }
  
  srand(time(NULL));
  //UnitTest();
  
  print_parameters();
  FILE* p;
  p = fopen("benchmark.txt", "a+");
  
  
  auto t_seq = Workload::run_sequential(num_threads * iteration_limit);
  auto t_cgh = Workload::run_coarse_grained();
  auto t_ch2 = Workload::run_concurrent();
  Workload::summarize(t_seq, t_cgh, t_ch2, p);
  
  fclose(p);
  return 0;
}



