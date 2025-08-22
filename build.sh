#!/bin/bash
# Скрипт для сборки проекта

mkdir -p ./.build
cd ./.build

cmake ..
cmake --build .