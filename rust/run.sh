#!/bin/bash

NUM_THREADS=$2
FLAG="$(($NUM_THREADS-1))"
cd $(readlink -f "$(dirname "$0")")
set -ex
source $HOME/.profile

echo ${NUM_THREADS}" threads"

if [ $NUM_THREADS == "1" ]
then
    RUSTFLAGS="-C target-cpu=native" taskset --cpu-list 0-$FLAG cargo run --release $1 1
else
    RUSTFLAGS="-C target-cpu=native" taskset --cpu-list 0-$FLAG cargo run --release $1 0
fi
