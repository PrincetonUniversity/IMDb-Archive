#!/usr/bin/bash

export PYTHONPATH=/usr/lib:$PYTHONPATH

cd oc8051
python genabst.py
cd oc8051-ila
mkdir -p build && cd build
cmake ..
make
./oc8051
