#!/bin/bash

g++ crack-skel.c -o crack
cat testfiles/cAliceby1.txt | ./crack 1 > output.out

diff -i testfiles/Aliceby1.out output.out
