#!/bin/bash

NUM_THREADS=$2
FLAG="$(($NUM_THREADS-1))"

cd $(readlink -f "$(dirname "$0")")
set -ex

javac Bench.java

echo ${NUM_THREADS}" threads"
taskset --cpu-list 0-$FLAG java Bench $1
