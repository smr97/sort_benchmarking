#include "pstl/algorithm"
#include "pstl/execution"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <parallel/algorithm>
#include <random>
#include <stdlib.h>
int NUM_RUNS = 100;

void write_results(
    std::string name, int num_threads,
    std::vector<std::chrono::duration<double, std::milli>> all_times) {
  std::ofstream outf;
  double mean_time = 0.0;
  outf.open(name + "_" + std::to_string(num_threads) + ".csv", std::ios::out);
  for (const auto _dur : all_times) {
    outf << _dur.count() / 1000.0 << std::endl;
    mean_time += _dur.count();
  }
  mean_time /= all_times.size();
  mean_time /= 1000.0;
  outf << "mean_time:" << mean_time << std::endl;
  outf.close();
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cout << "Usage <file> <problem_size> <num_threads>; where num_threads "
                 "is 1 for baselines and the real number of threads for "
                 "parallel algorithms"
              << std::endl;
    exit(1);
  }
  int len = atoi(argv[1]);
  int num_threads = atoi(argv[2]);

  std::vector<uint32_t> v;
  for (auto i = 0; i < len; i++) {
    v.push_back(i);
  }

  auto myinit = [&]() {
    auto rng = std::default_random_engine();
    std::shuffle(v.begin(), v.end(), rng);
  };

  if (num_threads == 1) {
    {
      std::vector<std::chrono::duration<double, std::milli>> times;
      for (int i = 0; i < NUM_RUNS; i++) {
        myinit();
        auto start = std::chrono::high_resolution_clock::now();
        std::stable_sort(pstl::execution::seq, v.begin(), v.end());
        auto end = std::chrono::high_resolution_clock::now();
        times.push_back(end - start);
      }
      write_results("cppsort_seq", num_threads, times);
    }
  } else {
    {
      std::vector<std::chrono::duration<double, std::milli>> times;
      for (int i = 0; i < NUM_RUNS; i++) {
        myinit();
        auto start = std::chrono::high_resolution_clock::now();
        std::stable_sort(pstl::execution::par, v.begin(), v.end());
        auto end = std::chrono::high_resolution_clock::now();
        times.push_back(end - start);
      }
      write_results("cppsort_pstl", num_threads, times);
    }
    {
      std::vector<std::chrono::duration<double, std::milli>> times;
      for (int i = 0; i < NUM_RUNS; i++) {
        myinit();
        auto start = std::chrono::high_resolution_clock::now();
        __gnu_parallel::stable_sort(v.begin(), v.end());
        auto end = std::chrono::high_resolution_clock::now();
        times.push_back(end - start);
      }
      write_results("cppsort_gnupar", num_threads, times);
    }
  }

  return 0;
}
