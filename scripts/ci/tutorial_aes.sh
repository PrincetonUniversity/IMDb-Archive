#!/bin/bash
export TUTORIAL_AES_DIR=$TRAVIS_BUILD_DIR/tutorials/aes
mkdir -p $TUTORIAL_AES_DIR/build && cd $TUTORIAL_AES_DIR/build
cmake ..
make
./AESExe
cd $TRAVIS_BUILD_DIR
