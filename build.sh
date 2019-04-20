#!/bin/bash

CUR_DIR=$(readlink -f .)
BUILD_DIR="_output"

case ${1:-debug} in
    debug)
        mkdir -p $BUILD_DIR
        cd $BUILD_DIR
        cmake -DCMAKE_BUILD_TYPE=Debug $CUR_DIR
        make
        ;;
    build)
        mkdir -p $BUILD_DIR
        cd $BUILD_DIR
        cmake -DCMAKE_BUILD_TYPE=Release $CUR_DIR
        make
        ;;
    clean)
        rm -rf $BUILD_DIR
        ;;
esac
