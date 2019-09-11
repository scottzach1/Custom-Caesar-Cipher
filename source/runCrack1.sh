#!/bin/bash

g++ crack-skel.c -o crack
cat cAliceby1.txt | ./crack 1 > output1.out

diff -i Aliceby1.out output1.out
