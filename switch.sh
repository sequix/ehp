#!/bin/sh

CURDIR="$(cd "$(dirname "$0")" >/dev/null && pwd)"

export C_INCLUDE_PATH="$CURDIR/src:$CURDIR/pkg/argtable3"
export CPLUS_INCLUDE_PATH="$CURDIR/src:$CURDIR/pkg/argtable3"

alias ehp-test='_output/ehp -c config-example'

