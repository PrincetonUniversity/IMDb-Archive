#!/usr/bin/bash
export GB_DIR=$TRAVIS_BUILD_DIR/accls/GB-Halide

# GB-L
cd $GB_DIR/ila_spec_b
python gb_hier.py
cd GB-ILA
mkdir -p build && cd build
cmake ..
make
./gb

