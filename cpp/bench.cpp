#include <chrono>
#include <stdlib.h>
#include <algorithm>
#include <random>
#include <iostream>
#include <parallel/algorithm>
#include "pstl/execution"
#include "pstl/algorithm"
int NUM_RUNS = 100;

void display(
  std::string name,
  double net_time_in_ms
) {
  std::cout << name;
  for (auto i = name.size(); i < 30; ++i) {
    std::cout << " ";
  }
  double avg_time = net_time_in_ms/(1000.0* double(NUM_RUNS));
  std::cout << avg_time;
  std::cout << " s" << std::endl;
}

int main(int argc, char* argv[]) {
    if(argc < 3){
        std::cout<<"Usage <file> <problem_size> <run_sequential>; where run_sequential is 1 for baselines and 0 for parallel algorithms"<<std::endl;
        exit(1);
    }
    int len = atoi(argv[1]);
    int run_sequential = atoi(argv[2]);

  std::vector<uint32_t> v;
  v.resize(len);

  auto myinit = [&] () {
    std::random_device device;
    std::mt19937_64 rnd(device());
    for (int i = 0; i < len; ++i) {
      v[i] = rnd();
    }
  };

  if (run_sequential>0){
    {
      auto total_sort_duration = std::chrono::duration<double, std::milli>::zero();
      for(int i = 0; i<NUM_RUNS; i++){
          myinit();
          auto start = std::chrono::high_resolution_clock::now();
          std::stable_sort(pstl::execution::seq, v.begin(), v.end());
          auto end = std::chrono::high_resolution_clock::now();
          std::chrono::duration<double, std::milli> sorting_time = end - start;
          total_sort_duration += sorting_time;
      }
      display("stable_sort+pstlseq", total_sort_duration.count());
    }
  } else {
    {
      auto total_sort_duration = std::chrono::duration<double, std::milli>::zero();
      for(int i = 0; i<NUM_RUNS; i++){
          myinit();
          auto start = std::chrono::high_resolution_clock::now();
          std::stable_sort(pstl::execution::par, v.begin(), v.end());
          auto end = std::chrono::high_resolution_clock::now();
          std::chrono::duration<double, std::milli> sorting_time = end - start;
          total_sort_duration += sorting_time;
      }
      display("stable_sort+pstlpar", total_sort_duration.count());
    }
    {
      auto total_sort_duration = std::chrono::duration<double, std::milli>::zero();
      for(int i = 0; i<NUM_RUNS; i++){
          myinit();
          auto start = std::chrono::high_resolution_clock::now();
          __gnu_parallel::stable_sort(v.begin(), v.end());
          auto end = std::chrono::high_resolution_clock::now();
          std::chrono::duration<double, std::milli> sorting_time = end - start;
          total_sort_duration += sorting_time;
      }
      display("__gnu_parallel::stable_sort", total_sort_duration.count());
    }
  }

  return 0;
}
