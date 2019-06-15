#!/usr/bin/bash

export PYTHONPATH=/usr/lib:$PYTHONPATH

# GB-L
cd ila_spec_b
python gb_hier.py
cd GB-ILA
mkdir -p build && cd build
cmake ..
make
./gb
