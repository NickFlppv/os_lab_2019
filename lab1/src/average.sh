#!/bin/bash
count=$#
echo "Number of parameters equals $count"
sum=0
while [ -n "$1" ]
do
    sum=$[$sum + $1]
    shift
done
average=$[$sum / $count]
echo "Average: $average"