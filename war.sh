#!/bin/bash

gcc algorithms/war.c -g #-fsanitize=address

#valgrind --leak-check=full ./a.out
#gdb ./a.out
./a.out

rm "/a.out"