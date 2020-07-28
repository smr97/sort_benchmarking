#!/bin/bash

PROBLEM_SIZE=$1
NUM_THREADS=$2
FLAG="$(($NUM_THREADS-1))"
cd $(readlink -f "$(dirname "$0")")
set -ex

echo ${NUM_THREADS}" threads"

RUSTFLAGS="-C target-cpu=native" taskset --cpu-list 0-$FLAG cargo run --release $PROBLEM_SIZE $NUM_THREADS
