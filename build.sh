#!/bin/bash

CURDIR=$(readlink -f .)
BUILD_DIR="_output"

function print_help
{
    echo 'usage: build.sh [test|debug|release|clean]'
}

case ${1:-debug} in
    debug)
        mkdir -p $BUILD_DIR
        cd $BUILD_DIR
        cmake -DCMAKE_BUILD_TYPE=Debug $CURDIR
        make
        ;;
    test)
        mkdir -p $BUILD_DIR
        cd $BUILD_DIR
        cmake -DCMAKE_BUILD_TYPE=Test $CURDIR
        if make; then
            make test || cat "Testing/Temporary/LastTest.log"
        fi
        ;;
    release)
        mkdir -p $BUILD_DIR
        cd $BUILD_DIR
        cmake -DCMAKE_BUILD_TYPE=Release $CURDIR
        make
        ;;
    clean)
        rm -rf $BUILD_DIR
        rm -f $CURDIR/config-example $CURDIR/ehp.log
        ;;
    *)
        print_help
        exit 1
        ;;
esac
