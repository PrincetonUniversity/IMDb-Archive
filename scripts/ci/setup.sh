#!/bin/bash
# test running aes tutorial
cd $TRAVIS_BUILD_DIR
export TUTORIAL_AES=$(pwd)/tutorials/aes
mkdir -p $TUTORIAL_AES/build && cd $TUTORIAL_AES/build
cmake ..
make
./AESExe
cd $TRAVIS_BUILD_DIR
