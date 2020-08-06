#!/bin/bash
PROBLEM_SIZE=100000000

rust/run.sh ${PROBLEM_SIZE} 1
for T in $(seq 2 4 64)
do
        rust/run.sh ${PROBLEM_SIZE} ${T}
done
