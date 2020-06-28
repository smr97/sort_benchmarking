#!/bin/bash
RUN_SCRIPTS=(*/run.sh)
MAX_THREADS=4
THREAD_INCREMENTS=1
PROBLEM_SIZE=1000000

for R in ${RUN_SCRIPTS[*]}
do
    for T in $(seq 1 ${THREAD_INCREMENTS} ${MAX_THREADS})
    do
            $R ${PROBLEM_SIZE} ${T}
    done
done

