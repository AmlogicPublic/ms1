#!/bin/bash
if [ -d build ]; then
    rm -rf build
fi
mkdir build
cd build

TOOLCHAIN_PATH="$(dirname "$0")/toolchain/gcc"
CMAKE_C_COMPILER="${TOOLCHAIN_PATH}/bin/riscv64-unknown-elf-gcc.exe"
CMAKE_CXX_COMPILER="${TOOLCHAIN_PATH}/bin/riscv64-unknown-elf-g++.exe"
NINJA_PATH="$(dirname "$0")/../../Software/Ninja.exe"

echo Using Ninja from: ${NINJA_PATH}

"$(dirname "$0")/../../../Software/cmake/bin/cmake.exe" -G "Ninja" -DCMAKE_MAKE_PROGRAM="${NINJA_PATH}" -DCMAKE_C_COMPILER="${CMAKE_C_COMPILER}" -DCMAKE_CXX_COMPILER="${CMAKE_CXX_COMPILER}" ..

if [ $? -eq 0 ]; then
    export NINJA_STATUS="[%s/%t] "
    "${NINJA_PATH}" 2>&1
else
    echo CMake configuration failed!
fi

cd ..