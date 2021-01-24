#!/bin/bash

ROOT="/home/marcus-s/Developer/Projects/systmatic/depot/systmatic/src/libsystm/src"
FLAGS="-fPIC -shared -std=c++11 -fpermissive -O2 -w"
INC="-I$ROOT"

SRC="\
$ROOT/fileio/fileio.cpp \
$ROOT/database/database.cpp \
$ROOT/database/databaserow.cpp \
"

OUTPUT="-o ./build/libsystm.so"

g++ $FLAGS $INC $SRC $OUTPUT
