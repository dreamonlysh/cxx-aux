#! /bin/bash

BUILD_TYPE=Release
BUILD_FOLDER=build_$(echo "${BUILD_TYPE}" | tr '[:upper:]' '[:lower:]')

cmake -B ${BUILD_FOLDER} \
  -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCXXAUX_ENABLE_TESTS=ON

cmake --build ${BUILD_FOLDER}

