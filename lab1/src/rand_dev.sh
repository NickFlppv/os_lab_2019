#!/bin/bash
for i in `seq $1`
do
    echo `< /dev/urandom tr -dc 1-9 | head -c5`
done