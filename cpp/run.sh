#!/bin/bash

NUM_THREADS=$2
FLAG="$(($NUM_THREADS-1))"
cd $(readlink -f "$(dirname "$0")")
set -ex

source /home/sraje/tbb-2020.3-lin/pstl/bin/pstlvars.sh intel64
g++ -flto -march=native -std=c++17 -ltbb -fopenmp -O2 bench.cpp -o bench

echo ${NUM_THREADS}" threads"
if [ $NUM_THREADS == "1" ]
then
    taskset --cpu-list 0-$FLAG ./bench $1 1
else
    taskset --cpu-list 0-$FLAG ./bench $1 0
fi
