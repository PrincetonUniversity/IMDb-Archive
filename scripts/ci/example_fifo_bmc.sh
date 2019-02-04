#!/bin/bash
export EXAMPLE_FIFO_BMC_DIR=$TRAVIS_BUILD_DIR/examples/FIFO_BMC
mkdir -p $EXAMPLE_FIFO_BMC_DIR/build && cd $EXAMPLE_FIFO_BMC_DIR/build
cmake ..
make
./fifo_bmc 20
cd $TRAVIS_BUILD_DIR
