#!/bin/bash
pwd > ~/Desktop/pwd1.txt
cd "graph_lib/lib/graph_lib_test"
pwd > ~/Desktop/pwd2.txt
set -e
make clean
make -f Makefile

