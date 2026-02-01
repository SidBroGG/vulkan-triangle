#!/bin/bash

# App
rm -rf ./build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build && cmake --build build

# Shaders
mkdir ./build/shaders
glslc shaders/shader.vert -o ./build/shaders/vert.spv
glslc shaders/shader.frag -o ./build/shaders/frag.spv

cd ./build
./vulkan-app
