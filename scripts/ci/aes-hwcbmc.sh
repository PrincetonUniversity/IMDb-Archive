#!/bin/bash

export AES_HWCBMC_DIR=$TRAVIS_BUILD_DIR/accls/AES/AES-RTL-C/HwCbmc
export AES_HWCBMC_DIR=$(pwd)/accls/AES/AES-RTL-C/HwCbmc
cd $AES_HWCBMC_DIR/src

scratch=$(mktemp -t tmp.XXXXXXXXXX)

../hw-cbmc aes_128.v --module aes_128 bug.c --bound 5 --unwind 30 > $scratch

function finish {
  echo "finish"
}

trap finish EXIT
