#!/bin/sh
#
rm -f triangle 
gcc triangle.c -o triangle -lraylib
./triangle
