##!/bin/sh

MAX_WINDOW=1024
CURRENT_WINDOW=1
RUN_DISK="python3 ./disk.py -A 1000,-1,0 -s 42 -p SATF -c -w"

while [ $CURRENT_WINDOW -le $MAX_WINDOW ]
    do
        echo "window size : $CURRENT_WINDOW" >> compare-window.test
        $RUN_DISK $CURRENT_WINDOW | tail -n 2 >> compare-window.test
        ((CURRENT_WINDOW*=2)) 
    done
