#!/bin/bash
RUN_SCRIPTS=(*/run.sh)
MAX_THREADS=64
THREAD_INCREMENTS=1
PROBLEM_SIZE=100000000

for R in ${RUN_SCRIPTS[*]}
do
    $R ${PROBLEM_SIZE} 1
    for T in $(seq 2 4 32)
    do
            $R ${PROBLEM_SIZE} ${T}
    done
    for T in $(seq 34 2 64)
    do
            $R ${PROBLEM_SIZE} ${T}
    done
done

