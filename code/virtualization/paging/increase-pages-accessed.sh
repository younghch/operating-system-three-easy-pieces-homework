#!/bin/bash

trials=$1
echo "running script for $trials trials"
for i in {0..18}
do
    ./tlb $((2**$i)) $trials >> "result-$trials.csv"
done