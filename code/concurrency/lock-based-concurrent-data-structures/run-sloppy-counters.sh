#!/bin/sh
# run sloppy-counter.out changing parameters.
#
# number of threads : 1 to 4
# increas count : fixed to 12000000
# threashold value: power of 2, exponet from 0 to 20
#

THREADS_RANGE=$(seq 4)
COUNT=12000000
EXPONENT=0
THREASHOLD=1
SLOPPY_COUNTER=./sloppy-counter.out

while [ $EXPONENT -le 21 ]
do
	for THREADS in $THREADS_RANGE
		do
			$SLOPPY_COUNTER $THREADS $COUNT $THREASHOLD >> sloppy_counter.test
		done
	((THREASHOLD*=2))
	((EXPONENT++))
done