# Minimum CMake Version and Project Definition
cmake_minimum_required(VERSION 3.10)

# Enable compile_commands.json for clangd and other tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Specify the cross compiler
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv32)

# Force cross-compilation settings to override any auto-detection
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Find architecture-specific multilib path
# i: Integer base instruction set.
# m: Integer multiplication and division.
# a: Atomic instructions.
# f: Single-precision floating point.
# d: Double-precision floating point.
# c: Compressed instructions.
# e: Reduced 32-bit embedded variant (fewer registers).
# zmmul: Multiply-only extension (subset of m).
# zaamo: Load-reserved/store-conditional atomic memory operations.
# zalrsc: Atomic LR/SC (Load-Reserved/Store-Conditional).
# zca, zcb, zcmp: Compressed instruction extensions for code size reduction.
# zba, zbb, zbs: Bit manipulation extensions.
# xxldsp: Likely refers to special DSP (Digital Signal Processing) optimizations.
set(TARGET "rv32imac_zaamo_zalrsc") # ARCH+ABI
set(ARCH "rv32imac")
set(ABI "ilp32")

# These flags tell the multilib toolchain to target 32-bit
set(ARCH_FLAGS "-march=${ARCH} -mabi=${ABI}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ARCH_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ARCH_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${ARCH_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${ARCH_FLAGS}")

# Specify the toolchain paths - using relative paths for better portability
set(TOOLCHAIN_PATH "${CMAKE_SOURCE_DIR}/toolchain/gcc")

# Verify toolchain exists
if(NOT EXISTS "${TOOLCHAIN_PATH}")
    message(FATAL_ERROR "Toolchain not found at: ${TOOLCHAIN_PATH}")
endif()

# Specify toolchain programs
set(CMAKE_C_COMPILER "${TOOLCHAIN_PATH}/bin/riscv64-unknown-elf-gcc.exe")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PATH}/bin/riscv64-unknown-elf-g++.exe")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PATH}/bin/riscv64-unknown-elf-gcc.exe")
set(CMAKE_OBJCOPY "${TOOLCHAIN_PATH}/bin/riscv64-unknown-elf-objcopy.exe")
set(CMAKE_OBJDUMP "${TOOLCHAIN_PATH}/bin/riscv64-unknown-elf-objdump.exe")
set(CMAKE_SIZE "${TOOLCHAIN_PATH}/bin/riscv64-unknown-elf-size.exe")
set(CMAKE_LINKER "${TOOLCHAIN_PATH}/bin/riscv64-unknown-elf-ld.exe")
set(CMAKE_AR "${TOOLCHAIN_PATH}/bin/riscv64-unknown-elf-ar.exe")
set(CMAKE_RANLIB "${TOOLCHAIN_PATH}/bin/riscv64-unknown-elf-ranlib.exe")
set(PYTHON_EXE "${CMAKE_SOURCE_DIR}/../../Software/python312/python.exe")
set(BIN2MEM_SCRIPT "${CMAKE_SOURCE_DIR}/bin/bin2mem.py")

# Cross-compilation settings moved to top of file

message(STATUS "Using RISC-V Toolchain: ${CMAKE_C_COMPILER}")

project(ms1 C CXX ASM) # this need to be later than project/toolchain config

# =============================================================================
# First ELF IMAGE: Main OS
# =============================================================================

# Compiler Library Configuration
# ===========================
# ==== C Runtime Library (CRT)
# +---------------------+-------------------------------+-------------------------------------------------------------+
# | Component           | Required Symbols              | Purpose                                                     |
# +---------------------+-------------------------------+-------------------------------------------------------------+
# | Startup Code        | _start, Reset_Handler         | First code executed (sets up stack, calls main).            |
# | Stack Setup         | _stack_end, _stack_start      | Defines stack memory region (linker script).                |
# | BSS Initialization  | _bss_start, _bss_end          | Zeroes uninitialized static/global variables (.bss section).|
# | DATA Initialization | _data_start, _data_end        | Copies initialized data from flash to RAM.                  |
# | Heap Management     | _heap_start, _heap_end        | Implements _sbrk for dynamic memory (malloc/free).          |
# | Syscall Stubs       | _write, _read, _close, _exit  | Provides low-level I/O for libc (e.g., printf → UART).      |
# | Compiler Helpers    | __udivdi3, __muldi3, __addsf3 | 64-bit math, FP emulation (from libgcc.a).   -lgcc          |
# | Interrupt Vectors   | irq_vector, trap_vector       | CPU exception/IRQ handlers (if applicable).                 |
# +---------------------+-------------------------------+-------------------------------------------------------------+
# Custom CRT = linker flag -nostartfiles + custom .S files for symbols needed + compiler helper -lgcc


# ==== Standard C Library (libc: glibc/newlib/musl)
# +------------------------+--------------------------------+------------------------------------------------------+
# | Component              | Required Symbols/Funcs         | Purpose                                              |
# +------------------------+--------------------------------+------------------------------------------------------+
# | Memory Management      | _sbrk, malloc, free            | Implements dynamic memory allocation (heap).         |
# | Standard I/O           | _write, _read, _close, _lseek  | Provides low-level I/O (e.g., printf, file I/O).     |
# | Process Control        | _exit, _kill, _getpid          | Minimal syscall stubs for bare-metal environments.   |
# | Time Functions         | _times, _gettimeofday          | Retrieves system time if implemented.                |
# | Environment Setup      | environ                        | Environment variable handling.                       |
# | Signal Handling        | _raise                         | Basic signal support for embedded systems.           |
# | File System Stubs      | _open, _fstat, _isatty (uart)  | Minimal file descriptors (often redirected to UART). |
# | Threading Support      | __malloc_lock, __malloc_unlock | Locks for reentrancy in multithreading.              |
# | Math Library           | sin, cos, pow, sqrt            | Floating-point and standard math functions.          |
# | String Handling        | strcpy, strcmp, memset         | Standard C string and memory functions.              |
# +--------------------------+------------------------------+------------------------------------------------------+

set(TOOLCHAIN_LIB_PATH "${TOOLCHAIN_PATH}/riscv64-unknown-elf/lib/${TARGET}/${ABI}")

set(CLIB_CONFIG "newlib-nano")  # newlib-nano / newlib
if(CLIB_CONFIG STREQUAL "newlib-nano")
    set(CLIB_SPECS "--specs=nano.specs")
    set(CLIB_C "-lc_nano")
else()
    set(CLIB_SPECS "")
    set(CLIB_C "-lc")
endif()

# Library Linking
# -Wl,--start-group/--end-group: Handle circular dependencies
# -lstdc++   : C++ standard library
# -lm        : Math library
# -lc_nano   : Nano version of C library
# -lgcc      : GCC support library (compilier helper, part of CRT core)
set(LIBS -Wl,--start-group -lstdc++ -lm ${CLIB_C} -lgcc -Wl,--end-group)

# Compiler Flags Configuration
# ===========================

# Common Compilation Flags
# -march=rv32imac           : Target 32-bit RISC-V with Integer, Multiply/Divide, Atomic, Compressed instruction sets
# -mabi=ilp32               : Use ILP32 ABI (32-bit integers, longs, pointers)
# -mtune=nuclei-200-series  : Optimize for Nuclei 200-series RISC-V core
# -mcmodel=medlow           : Relocation model:
#                               medlow: 0~2GiB,     absolute addressing,    for statically linked exe
#                               medany: PC+-2GIB,   PC-relative addressing, for PIC(position-independent code) and shared libraries
# Use medium-low code model (up to 2MB memory) (further address brach will result in compiler error)
# -mno-save-restore         : Disable automatic save/restore of registers
# -O0/1/2/3/s/fast/g/z      : Disable optimizations for debugging (s/z for smaller size)
# -g                        : Include debug symbols
set(COMMON_FLAGS "-march=${ARCH} -mabi=${ABI} ${CLIB_SPECS} -mtune=nuclei-200-series -mcmodel=medlow -mno-save-restore -Os -g")

# C Compiler Flags
# -ffunction-sections : Place each function in its own section for linker optimization
# -fdata-sections     : Place each data item in its own section
# -fno-common         : Prevent merging of uninitialized globals
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS} -ffunction-sections -fdata-sections -fno-common")

# C++ Compiler Flags (same as C flags)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} -ffunction-sections -fdata-sections -fno-common")

# Assembly Compiler Flags
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${COMMON_FLAGS}")

# Boot Configuration
# ==================
# Configure boot method and associated linker script/startup file
# Options:
#   FLASH_BOOT: Boot from flash, copy code/data to ILM/DLM
#   ILM_BOOT:   Boot from ILM, assume code/data already loaded in ILM
#   SRAM_BOOT:  Boot from SRAM, assume code/data already loaded in SRAM
# Note: ILM_BOOT and SRAM_BOOT share the same startup file (startup_from_sram.S)
#       since both assume code is already in RAM and skip copy operations
set(BOOT_METHOD "SRAM_BOOT")

# Set linker script and startup file based on boot method
if(BOOT_METHOD STREQUAL "FLASH_BOOT")
    set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/soc/ms1/source/GCC/ms1_flash_boot.ld")
    set(STARTUP_FILE "${CMAKE_SOURCE_DIR}/soc/ms1/source/GCC/startup_from_flash.S")
    message(STATUS "Boot Method: Flash Boot - Copy from flash to ILM/DLM")
elseif(BOOT_METHOD STREQUAL "ILM_BOOT")
    set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/soc/ms1/source/GCC/ms1_ilm_boot.ld")
    set(STARTUP_FILE "${CMAKE_SOURCE_DIR}/soc/ms1/source/GCC/startup_from_sram.S")
    message(STATUS "Boot Method: ILM Boot - Direct execution from ILM")
elseif(BOOT_METHOD STREQUAL "SRAM_BOOT")
    set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/soc/ms1/source/GCC/ms1_sram_boot.ld")
    set(STARTUP_FILE "${CMAKE_SOURCE_DIR}/soc/ms1/source/GCC/startup_from_sram.S")
    message(STATUS "Boot Method: SRAM Boot - Direct execution from SRAM")
else()
    message(FATAL_ERROR "Invalid BOOT_METHOD: ${BOOT_METHOD}. Must be FLASH_BOOT, ILM_BOOT, or SRAM_BOOT")
endif()

# Verify that the selected files exist
if(NOT EXISTS "${LINKER_SCRIPT}")
    message(FATAL_ERROR "Linker script not found: ${LINKER_SCRIPT}")
endif()
if(NOT EXISTS "${STARTUP_FILE}")
    message(FATAL_ERROR "Startup file not found: ${STARTUP_FILE}")
endif()

# Linker Flags Configuration
# =========================

# Linker Script and Basic Linker Flags
# -T                       : Specify custom linker script
# -nostartfiles            : Exclude standard startup files
# -nodefaultlibs           : Exclude default standard libraries(libgcc, libc, libm, libstdc++, ...)
# -Xlinker --gc-sections   : Enable garbage collection of unused sections
# -Wl,-Map                 : Generate memory map file
# -Wl,--check-sections         # Ensures linker checks section alignments and overlaps
# -Wl,--no-warn-rwx-segments   # Suppresses warnings about segments with read, write, and execute permissions
set(LINKER_FLAGS
    -T${LINKER_SCRIPT}
    -nostartfiles
    -L${TOOLCHAIN_PATH}/lib/gcc/riscv64-unknown-elf/14.2.1/${TARGET}/${ABI}
    -L${TOOLCHAIN_LIB_PATH}
    -L${CMAKE_SOURCE_DIR}/soc/ms1/source/GCC
    -Wl,-Map,${CMAKE_BINARY_DIR}/ms1.map
    -Wl,--gc-sections
    -Wl,--check-sections
    -Wl,--no-warn-rwx-segments
)

# Executable Target
# ================
# Collect C/C++ sources
file(GLOB_RECURSE C_SOURCES
    # "${CMAKE_SOURCE_DIR}/application/main_BareMetal.c"
    "${CMAKE_SOURCE_DIR}/application/main_FreeRTOS.c"
    "${CMAKE_SOURCE_DIR}/application/vpu_run.c"
    "${CMAKE_SOURCE_DIR}/application/*.cpp"
    "${CMAKE_SOURCE_DIR}/soc/ms1/source/*.c"
    "${CMAKE_SOURCE_DIR}/soc/ms1/source/Drivers/*.c"
    "${CMAKE_SOURCE_DIR}/soc/ms1/source/Stubs/newlib/*.c"
    "${CMAKE_SOURCE_DIR}/kernel/freertos/*.c"
    "${CMAKE_SOURCE_DIR}/kernel/freertos/portable/*.c"
    "${CMAKE_SOURCE_DIR}/kernel/freertos/portable/MemMang/*.c"
)

# Collect FreeRTOS assembly sources
file(GLOB_RECURSE FREERTOS_ASM_SOURCES
    "${CMAKE_SOURCE_DIR}/kernel/freertos/portable/GCC/RISC-V/*.S"
)

# Combine all sources: C/C++ sources + selected startup file + FreeRTOS assembly
set(SOURCES ${C_SOURCES} ${STARTUP_FILE} ${FREERTOS_ASM_SOURCES})
add_executable(ms1 ${SOURCES})
set_target_properties(ms1 PROPERTIES OUTPUT_NAME "ms1" SUFFIX ".elf")

# Include Directories
# ==================
target_include_directories(ms1 PRIVATE
    # Compiler headers + CRT (C Runtime) (crt.o, libgcc.a, ...)
    ${TOOLCHAIN_PATH}/lib/gcc/riscv64-unknown-elf/14.2.1/include                                   # Contains GCC-specific headers, including built-in macros and compiler intrinsics.
    ${TOOLCHAIN_PATH}/lib/gcc/riscv64-unknown-elf/14.2.1/include-fixed                             # Holds "fixed" versions of system headers that have been patched by GCC 
    
    # libc (C Standard Library)
    ${TOOLCHAIN_PATH}/riscv64-unknown-elf/include
    ${TOOLCHAIN_PATH}/riscv64-unknown-elf/include/newlib-nano                                      # Headers from Newlib Nano, a lightweight C library optimized for embedded systems
    # ${TOOLCHAIN_GCC_INCLUDE}

    # C++ Standard Library (libstdc++)
    ${TOOLCHAIN_PATH}/riscv64-unknown-elf/include/c++/14.2.1                                       # Standard C++ headers for this GCC version (14.2.1).
    ${TOOLCHAIN_PATH}/riscv64-unknown-elf/include/c++/14.2.1/backward                              # Legacy C++ headers for backward compatibility
    ${TOOLCHAIN_PATH}/riscv64-unknown-elf/include/c++/14.2.1/riscv64-unknown-elf/${TARGET}/${ABI}  # Target-specific(ARCH+ABI) C++ headers for 

    # OS/App Libraries
    ${CMAKE_SOURCE_DIR}/application
    ${CMAKE_SOURCE_DIR}/nmsis/core/include
    ${CMAKE_SOURCE_DIR}/soc/ms1/include
    ${CMAKE_SOURCE_DIR}/kernel/freertos/include
    ${CMAKE_SOURCE_DIR}/kernel/freertos/portable

    # Drivers
    ${CMAKE_SOURCE_DIR}/soc/ms1/include/Drivers/edp
    ${CMAKE_SOURCE_DIR}/soc/ms1/include/Drivers/vpu
)

target_link_options(ms1 PRIVATE ${LINKER_FLAGS} ${LIBS})

# Post-Build Commands
# ==================
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/ms1.bin
    DEPENDS ms1
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:ms1> "${CMAKE_BINARY_DIR}/ms1.hex"
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:ms1> "${CMAKE_BINARY_DIR}/ms1.bin"
    COMMAND ${PYTHON_EXE} ${BIN2MEM_SCRIPT} ${CMAKE_BINARY_DIR}/ms1.bin ${CMAKE_BINARY_DIR}/ms1.mem
    COMMAND ${CMAKE_OBJDUMP} --source --all-headers --demangle --line-numbers --wide $<TARGET_FILE:ms1> > "${CMAKE_BINARY_DIR}/ms1.lst"
    COMMAND ${CMAKE_SIZE} -A $<TARGET_FILE:ms1>
    COMMENT "Image(Main): Generating hex file, disassembly listing, and size information"
)

# =============================================================================
# Dependency
# =============================================================================
# ALL = built by default, other wise dependency is parsed and only needed ones are built
add_custom_target(ms1_bin               ALL DEPENDS ${CMAKE_BINARY_DIR}/ms1.bin             )

add_custom_command(TARGET ms1 POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -h $<TARGET_FILE:ms1>
    COMMAND ${CMAKE_READELF} -S $<TARGET_FILE:ms1>
    COMMENT "Displaying ELF section information"
)
