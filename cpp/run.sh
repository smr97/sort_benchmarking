#!/bin/bash

PROBLEM_SIZE=$1
NUM_THREADS=$2
FLAG="$(($NUM_THREADS-1))"
cd $(readlink -f "$(dirname "$0")")
set -ex

g++ -flto -march=native -std=c++17 -O2 bench.cpp -ltbb -fopenmp -o bench

echo ${NUM_THREADS}" threads"
taskset --cpu-list 0-$FLAG ./bench $PROBLEM_SIZE $NUM_THREADS
