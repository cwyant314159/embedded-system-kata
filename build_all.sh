#!/bin/bash
#
# Build all exercises

set -e # exit on errors

FINAL_EXERCISE=7

for i in $(seq 0 $FINAL_EXERCISE); do
    KATA=$i make build
done