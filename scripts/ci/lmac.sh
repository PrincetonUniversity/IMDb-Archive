#!/usr/bin/bash
export LMAC_DIR=$TRAVIS_BUILD_DIR/uncores/lmac

# LMAC Core2
cd $LMAC_DIR
mkdir -p build && cd build
cmake ..
make
