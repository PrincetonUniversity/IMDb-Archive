#!/bin/bash

# running tutorial
cd $TRAVIS_BUILD_DIR
export AES_ROOT=$(pwd)/tutorials/aes
mkdir -p $AES_ROOT/build
cd $AES_ROOT/build
cmake ..
make
./AESExe
cd $AES_ROOT/verification
#./RunAll.sh
cd $TRAVIS_BUILD_DIR
