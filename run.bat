@echo off
if exist build rmdir /s /q build
mkdir build
cd build

set TOOLCHAIN_PATH=%~dp0toolchain\gcc
set CMAKE_C_COMPILER=%TOOLCHAIN_PATH%\bin\riscv64-unknown-elf-gcc.exe
set CMAKE_CXX_COMPILER=%TOOLCHAIN_PATH%\bin\riscv64-unknown-elf-g++.exe
set NINJA_PATH=%~dp0..\..\Software\Ninja.exe

echo Using Ninja from: %NINJA_PATH%

..\..\..\Software\cmake\bin\cmake.exe -G "Ninja" -DCMAKE_MAKE_PROGRAM="%NINJA_PATH%" -DCMAKE_C_COMPILER="%CMAKE_C_COMPILER%" -DCMAKE_CXX_COMPILER="%CMAKE_CXX_COMPILER%" ..

if %ERRORLEVEL% EQU 0 (
    set NINJA_STATUS=[%%s/%%t] 
    call "%NINJA_PATH%" 2>&1 | powershell -Command "$input -replace '\r', \"`n\""
) else (
    echo CMake configuration failed!
)

cd ..