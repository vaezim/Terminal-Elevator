#!/usr/bin/env bash

PROJECT_ROOT_DIR=$(pwd)
BUILD_TYPE=Relase

cmake                                   \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE}    \
    -S ${PROJECT_ROOT_DIR}              \
    -B ${PROJECT_ROOT_DIR}/output/build

make --no-print-directory -C ${PROJECT_ROOT_DIR}/output/build -j4