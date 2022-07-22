#!/bin/sh
# run compare-list.out changing parameters.
#
# size of list : 128 * power of 8, exponet from 1 to 4
# number of threads : 1, 2, 4, 8
# search count : size of list / 16 
#

LIST_SIZE=1024
EXPONENT=1
THREADS=(1 2 4 8)
COMPARE_LIST=./compare-list.out
while [ $EXPONENT -le 4 ]
	do
		for THREAD in "${THREADS[@]}"
			do
				$COMPARE_LIST $LIST_SIZE $THREAD $((LIST_SIZE/16)) >> compare-list.test
			done
		((LIST_SIZE*=4))
		((EXPONENT++))
	done