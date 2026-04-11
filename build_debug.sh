#! /bin/bash

BUILD_TYPE=Debug
BUILD_FOLDER=build_$(echo "${BUILD_TYPE}" | tr '[:upper:]' '[:lower:]')

cmake -B ${BUILD_FOLDER} \
  -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
  -DCXXAUX_ENABLE_TESTS=ON

cmake --build ${BUILD_FOLDER}

ctest --test-dir ${BUILD_FOLDER}

