#!/bin/bash

g++ crack-skel.c -o crack
cat cAliceby2.txt | ./crack 2 > output2.out

diff -i Aliceby2.out output2.out
