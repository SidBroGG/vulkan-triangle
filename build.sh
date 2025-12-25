#!/bin/bash

rm -rf ./build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build && cmake --build build
./build/vulkan-app
