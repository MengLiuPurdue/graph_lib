#!/bin/bash

cd graph_lib/lib/graph_lib_test

set -e
make clean
make -f Makefile

lib=$(find . -type f | grep libgraph)
loc=$(find ../../../build -type d -name "graph_lib_test")

cp ${lib} ${loc}
