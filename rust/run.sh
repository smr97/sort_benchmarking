#!/bin/bash

NUM_THREADS=$2
FLAG="$(($NUM_THREADS-1))"
cd $(readlink -f "$(dirname "$0")")
set -ex

echo ${NUM_THREADS}" threads"
cargo build --release
if [ $NUM_THREADS == "1" ]
then
    taskset --cpu-list 0-$FLAG cargo run --release $1 1
else
    taskset --cpu-list 0-$FLAG cargo run --release $1 0
fi
