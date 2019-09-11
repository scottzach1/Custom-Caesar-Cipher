#!/bin/bash

g++ testfiles/encode.c -o encode
g++ testfiles/decode.c -o decode
cat testfiles/output.out | ./encode "i came, i saw" | ./decode "i came, i saw"
