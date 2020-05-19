#!/bin/bash

if [ $# -eq 3 ]
then
    ./build/efs -o nonempty $1 $2 $3
else
    echo "Not enough arguments"
fi