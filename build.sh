#!/bin/bash

CUR_DIR=$(readlink -f .)
BUILD_DIR="_output"

case ${1:-build} in
    build)
        mkdir -p $BUILD_DIR
        cd $BUILD_DIR
        cmake $CUR_DIR
        make
        ;;
    clean)
        rm -rf $BUILD_DIR
        ;;
esac
