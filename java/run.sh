#!/bin/bash

NUM_THREADS=$2
FLAG="$(($NUM_THREADS-1))"

cd $(readlink -f "$(dirname "$0")")
set -ex

javac Bench.java

echo ${NUM_THREADS}" threads"
if [ $NUM_THREADS == "1" ]
then
    taskset --cpu-list 0-$FLAG java Bench $1 1
else
    taskset --cpu-list 0-$FLAG java Bench $1 0
fi
