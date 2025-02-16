#!/usr/bin/env sh

export CMAKE_EXPORT_COMPILE_COMMANDS=1
mkdir -p build && cd build
cmake ..
cmake --build .

