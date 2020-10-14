#!/bin/bash

INC="-I../libsystm/src -I./src/client/src"
FLAGS="-std=c++11 -w -O2 -fpermissive"
OUTPUT="-o build/client/systmpack"
LIBFOLDER="-L../libsystm/build"
LIBS="-lsystm -lz"

ROOT="/home/marcus-s/Developer/systmatic/src/systmpack/src/client/src"

SRC="\
$ROOT/main.cpp \
$ROOT/packagedb.cpp \
"

g++ $INC $FLAGS $LIBFOLDER $SRC $OUTPUT $LIBS
