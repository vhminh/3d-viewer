#!/bin/bash

mkdir -p build && cd build && cmake .. && make && cd .. && ./build/learnopengl
