#!/bin/bash
mkdir -p build && cd build
cmake ..
make
./fifo_bmc 20
