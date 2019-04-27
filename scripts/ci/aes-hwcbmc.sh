#!/bin/bash

export AES_HWCBMC_DIR=$TRAVIS_BUILD_DIR/accls/AES/AES-RTL-C/HwCbmc
cd $AES_HWCBMC_DIR/src
../hw-cbmc aes_128.v --module aes_128 bug.c --bound 5 --unwind 30
