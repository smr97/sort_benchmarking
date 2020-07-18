#!/bin/bash

NUM_THREADS=$2
FLAG="$(($NUM_THREADS-1))"
cd $(readlink -f "$(dirname "$0")")
set -ex

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/grid5000/spack/opt/spack/linux-debian9-x86_64/gcc-6.4.0/intel-tbb-2019.2-q2t24fbv6s4d5x2j5jaykuqrdkec75zq/lib
export LIBRARY_PATH=$LIBRARY_PATH:$LD_LIBRARY_PATH
g++ -march=native -O2 bench.cpp -o bench -std=c++14 -fopenmp -ltbb -I/grid5000/spack/opt/spack/linux-debian9-x86_64/gcc-6.4.0/intel-tbb-2019.2-q2t24fbv6s4d5x2j5jaykuqrdkec75zq/include/

echo ${NUM_THREADS}" threads"
if [ $NUM_THREADS == "1" ]
then
    taskset --cpu-list 0-$FLAG ./bench $1 1
else
    taskset --cpu-list 0-$FLAG ./bench $1 0
fi
