#!/bin/bash

time hw-cbmc aes_128.v --module aes_128 bug.c --bound 22 --unwind 30 > ../logs/bug.log
time hw-cbmc aes_128.v --module aes_128 wrong_map.c --bound 22 --unwind 25 --no-unwinding-assertions > ../logs/sat.log
