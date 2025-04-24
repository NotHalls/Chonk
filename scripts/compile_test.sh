#!/bin/bash
set -e

# Root build directory
BUILD_DIR="test/build"

echo "Select A Build Type: 'Debug; Release; Export'"
read BUILD_TYPE

# Function to build with a specific compiler
build_with() {
  COMPILER=$1
  GENERATOR=$2
  C_COMPILER=$3
  CXX_COMPILER=$4

  DIR="../${BUILD_DIR}/${BUILD_TYPE}-${COMPILER}"
  echo "⚙️ Building with ${COMPILER}..."

  cmake .. -DCMAKE_C_COMPILER="$C_COMPILER" -DCMAKE_CXX_COMPILER="$CXX_COMPILER" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -B "$DIR" -G "$GENERATOR"
  cmake --build "$DIR"
}

# Clang build
build_with "Clang" "Ninja" "clang" "clang++"
# GCC build
build_with "GNU" "Ninja" "gcc" "g++"
# MSVC build
build_with "MSVC" "Visual Studio 17 2022" "MSVC" "MSVC"