#!/bin/bash

g++ crack-skel.c -o crack
cat testfiles/cAliceby2.txt | ./crack 2 > output.out

diff -i testfiles/Aliceby2.out output.out
