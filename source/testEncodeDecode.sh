#!/bin/bash

g++ encode.c -o encode
g++ decode.c -o decode
cat test.txt | ./encode "i came, i saw" | ./decode "i came, i saw"
