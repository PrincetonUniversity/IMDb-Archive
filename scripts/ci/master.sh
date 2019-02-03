#!/bin/bash
export CI_TEST_SCRIPTS=$TRAVIS_BUILD_DIR/scripts/ci

# test aes tutorial
source $CI_TEST_SCRIPTS/tutorial_aes.sh

