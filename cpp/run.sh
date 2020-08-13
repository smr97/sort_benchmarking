#!/bin/bash

PROBLEM_SIZE=$1
NUM_THREADS=$2
FLAG="$(($NUM_THREADS-1))"
cd $(readlink -f "$(dirname "$0")")
set -ex

#source /home/sraje/tbb-2020.3-lin/pstl/bin/pstlvars.sh intel64
g++ -flto -march=native -std=c++17 -O2 bench.cpp -ltbb -fopenmp -o bench

echo ${NUM_THREADS}" threads"
if [ $NUM_THREADS == "1" ]
then
    exit 0
else
    taskset --cpu-list 0-$FLAG ./bench $PROBLEM_SIZE $NUM_THREADS
fi
