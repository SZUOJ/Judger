#!/bin/bash
set -ex

mkdir -p build
cd build
make
make install
cd ..
cd Judger/bindings
sh bind.sh