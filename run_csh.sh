#!/bin/csh
if ( -d build ) then
    rm -rf build
endif
mkdir build
cd build

set TOOLCHAIN_PATH = "$0:h/toolchain/gcc"
set CMAKE_C_COMPILER = "${TOOLCHAIN_PATH}/bin/riscv64-unknown-elf-gcc.exe"
set CMAKE_CXX_COMPILER = "${TOOLCHAIN_PATH}/bin/riscv64-unknown-elf-g++.exe"
set NINJA_PATH = "$0:h/../../Software/Ninja.exe"

echo Using Ninja from: ${NINJA_PATH}

"$0:h/../../../Software/cmake/bin/cmake.exe" -G "Ninja" -DCMAKE_MAKE_PROGRAM="${NINJA_PATH}" -DCMAKE_C_COMPILER="${CMAKE_C_COMPILER}" -DCMAKE_CXX_COMPILER="${CMAKE_CXX_COMPILER}" ..

if ( $status == 0 ) then
    setenv NINJA_STATUS "[%s/%t] "
    "${NINJA_PATH}" >& /dev/stdout
else
    echo CMake configuration failed!
endif

cd ..
