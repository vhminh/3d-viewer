#!/bin/bash

export CMAKE_EXPORT_COMPILE_COMMANDS=1
mkdir -p build && cd build && cmake .. && make && cd .. && ./build/learnopengl

