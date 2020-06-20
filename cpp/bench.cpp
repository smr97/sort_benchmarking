#include <chrono>
#include <stdlib.h>
#include <algorithm>
#include <random>
#include <iostream>
#include <parallel/algorithm>
#include <tbb/parallel_sort.h>
#include <tbb/task_scheduler_init.h>
int NUM_RUNS = 100;

void display(
  std::string name,
  std::chrono::time_point<std::chrono::high_resolution_clock> start,
  std::chrono::time_point<std::chrono::high_resolution_clock> end
) {
  std::cout << name;
  for (auto i = name.size(); i < 30; ++i) {
    std::cout << " ";
  }
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  std::cout << " ms" << std::endl;
}

int main(int argc, char* argv[]) {
    if(argc < 3){
        std::cout<<"Usage <file> <problem_size> <num_threads>"<<std::endl;
        exit(1);
    }
    int len = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    tbb::task_scheduler_init init(num_threads);
    setenv("OMP_NUM_THREADS", argv[2], 1);

  std::vector<uint64_t> v;
  v.resize(len);

  auto myinit = [&] () {
    std::random_device device;
    std::mt19937_64 rnd(device());
    for (int i = 0; i < len; ++i) {
      v[i] = rnd();
    }
  };

  {
    for(int i = 0; i<NUM_RUNS; i++){
    myinit();
    auto start = std::chrono::high_resolution_clock::now();
    std::stable_sort(v.begin(), v.end());
    auto end = std::chrono::high_resolution_clock::now();
    display("std::stable_sort", start, end);
    }
  }
  {
    for(int i = 0; i<NUM_RUNS; i++){
    myinit();
    auto start = std::chrono::high_resolution_clock::now();
    std::sort(v.begin(), v.end());
    auto end = std::chrono::high_resolution_clock::now();
    display("std::sort", start, end);
    }
  }
  {
    for(int i = 0; i<NUM_RUNS; i++){
    myinit();
    auto start = std::chrono::high_resolution_clock::now();
    __gnu_parallel::stable_sort(v.begin(), v.end());
    auto end = std::chrono::high_resolution_clock::now();
    display("__gnu_parallel::stable_sort", start, end);
    }
  }
  {
    for(int i = 0; i<NUM_RUNS; i++){
    myinit();
    auto start = std::chrono::high_resolution_clock::now();
    __gnu_parallel::sort(v.begin(), v.end());
    auto end = std::chrono::high_resolution_clock::now();
    display("__gnu_parallel::sort", start, end);
    }
  }
  {
    for(int i = 0; i<NUM_RUNS; i++){
    myinit();
    auto start = std::chrono::high_resolution_clock::now();
    tbb::parallel_sort(v.begin(), v.end());
    auto end = std::chrono::high_resolution_clock::now();
    display("tbb::parallel_sort", start, end);
    }
  }

  return 0;
}
