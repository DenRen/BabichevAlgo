#!/bin/bash

for (( i=1; i<=4; ++i ))
do

    ./a.out < $i > .${i}r
    diff .${i}r ${i}_solve
    
    rm .${i}r
done
