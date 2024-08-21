#!/bin/bash
gcc gravler.c -fopenmp -o gravler.out

./gravler.out 10000
./gravler.out 50000
./gravler.out 100000
./gravler.out 200000