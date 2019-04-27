#!/bin/bash
hw-cbmc aes_128.v --module aes_128 harness.c --bound 11 --unwind 20
