#!/bin/bash

NUM_THREADS=$2
FLAG="$(($NUM_THREADS-1))"
cd $(readlink -f "$(dirname "$0")")
set -ex

echo "******** RUST SORTS ********"
cargo build --release
taskset --cpu-list 0-$FLAG cargo run --release $1
