#!/bin/bash

# running tutorial
cd $TRAVIS_BUILD_DIR
set AES_ROOT=$(pwd)/tutorials/aes
cd $AES_ROOT/build
cmake ..
make
./AESExe
cd $AES_ROOT/verification
#./RunAll.sh
cd $TRAVIS_BUILD_DIR
