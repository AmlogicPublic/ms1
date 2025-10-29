#!/bin/bash

# https://doc.nucleisys.com/nuclei_tools/qemu/intro.html#design-and-architecture
# 
# emulation modes:
# 1. mock mode: download mode = Flash, image = main.elf on ilm
# 2. real mode: download mode = FlashXIP, image = bootloader.elf on flash + main.elf on flash
# 
# download modes:
# +--------------------------+-----------------------------------------------------------------------------------------------------------+
# | ILM download mode        | program will be downloaded into ilm/ram and run directly in ilm/ram, program lost when poweroff           |
# +--------------------------+-----------------------------------------------------------------------------------------------------------+
# | FLASH download mode      | program will be downloaded into flash, when running, program will be copied to ilm/ram and run in ilm/ram |
# +--------------------------+-----------------------------------------------------------------------------------------------------------+
# | FLASHXIP download mode   | program will be downloaded into flash and run directly in flash                                           |
# +--------------------------+-----------------------------------------------------------------------------------------------------------+
# | DDR download mode        | program will be downloaded into ddr and run directly in ddr, program lost when poweroff                   |
# +--------------------------+-----------------------------------------------------------------------------------------------------------+
# | SRAM download mode       | (gram mode use sram for 200/300, ddr for 600/900)                                                         |
# +--------------------------+-----------------------------------------------------------------------------------------------------------+
# 
# C library support
# +----------------------+------------------------------------------------------------------------------------------+
# | newlib_full          | newlib with full feature                                                                 |
# +----------------------+------------------------------------------------------------------------------------------+
# | newlib_fast          | newlib nano with printf/scanf float                                                      |
# +----------------------+------------------------------------------------------------------------------------------+
# | newlib_small         | newlib nano with printf float                                                            |
# +----------------------+------------------------------------------------------------------------------------------+
# | newlib_nano          | newlib nano without printf/scanf float                                                   |
# +----------------------+------------------------------------------------------------------------------------------+
# | libnert_fast         | nuclei c runtime library, optimized for speed                                            |
# +----------------------+------------------------------------------------------------------------------------------+
# | libnert_balanced     | nuclei c runtime library, balanced, full feature                                         |
# +----------------------+------------------------------------------------------------------------------------------+
# | libnert_small        | nuclei c runtime library, optimized for size, full feature                               |
# +----------------------+------------------------------------------------------------------------------------------+
# | libnert_nano         | nuclei c runtime library, optimized for size, no float support                           |
# +----------------------+------------------------------------------------------------------------------------------+
# | libnert_pico         | nuclei c runtime library, optimized for size, no long/long long support                  |
# +----------------------+------------------------------------------------------------------------------------------+
# | nostd                | no std c library will be used, and don't search the standard system directories for      |
# |                      | header files                                                                             |
# +----------------------+------------------------------------------------------------------------------------------+
# | nospec               | no std c library will be used, not pass any --specs options                              |
# +----------------------+------------------------------------------------------------------------------------------+
# 
# evalSoC:
# +----------------------+--------------+------------+---------------------------------+
# |      Component       | Base Address |    Size    |          Description            |
# +----------------------+--------------+------------+---------------------------------+
# | Memory Resource                                                                    |
# +----------------------+--------------+------------+---------------------------------+
# | XIP                  | 0x20000000   | 0x02000000 | XIP address space.              |
# | DDR                  | 0x80000000   | 0x04000000 | DDR address space.              |
# | ILM                  | 0x80000000   | 0x00800000 | ILM address space.              |
# | DLM                  | 0x90000000   | 0x00800000 | DLM address space.              |
# | SRAM                 | 0xA0000000   | 0x20000000 | SRAM address space.             |
# +----------------------+--------------+------------+---------------------------------+
# | Peripherals (Fixed)                                                                |
# +----------------------+--------------+------------+---------------------------------+
# | IINFO                | 0x00000000   | 0x00001000 | IINFO address space.            |
# | MROM                 | 0x00001000   | 0x0000F000 | MROM address space.             |
# | TEST                 | 0x00100000   | 0x00010000 | Eval_SoC exit mechanism.        |
# | GPIO                 | 0x10012000   | 0x00001000 | GPIO address space.             |
# | UART0                | 0x10013000   | 0x00001000 | UART0 address space.            |
# | UART1                | 0x10023000   | 0x00001000 | UART1 address space.            |
# | QSPI0                | 0x10014000   | 0x00001000 | QSPI0 address space.            |
# | QSPI1                | 0x10024000   | 0x00001000 | QSPI1 address space.            |
# | QSPI2                | 0x10034000   | 0x00001000 | QSPI2 address space.            |
# +----------------------+--------------+------------+---------------------------------+
# | Peripherals (Offset)                                                               |
# +----------------------+--------------+------------+---------------------------------+
# | DEBUG                | 0x00010000   | 0x00001000 | DEBUG address space.            |
# | ECLIC                | 0x00020000   | 0x00010000 | ECLIC address space.            |
# | TIMER                | 0x00030000   | 0x00010000 | TIMER address space.            |
# | SMP                  | 0x00040000   | 0x00001000 | SMP address space.              |
# | CIDU                 | 0x00050000   | 0x00010000 | CIDU address space.             |
# | PLIC                 | 0x04000000   | 0x04000000 | PLIC address space.             |
# | PPI                  | 0xB0000000   | /          | CPU info (not implemented)      |
# | FIO                  | 0xC0000000   | /          | CPU info (not implemented)      |
# +----------------------+--------------+------------+---------------------------------+
# 
# Features:
# +----------------------+----------------------+---------------------------------------------------------------------+
# |    CPU Features      |   Status on QEMU     |                       Description                                   |
# +----------------------+----------------------+---------------------------------------------------------------------+
# | NMI                  | Not supported        | Non-Maskable Interrupt for critical, high-priority events           |
# | TIMER                | Supported            | Hardware timer generating periodic interrupts                       |
# | PLIC                 | Supported            | Platform-Level Interrupt Controller for managing interrupts         |
# | ECLIC                | Supported            | Enhanced Core-Local Interrupt Controller for advanced handling      |
# | CIDU                 | Supported            | Core Interrupt Distribution Unit for routing interrupts             |
# | PMP                  | Supported            | Physical Memory Protection enforcing access policies                |
# | TEE                  | Only CSRs Supported  | Trusted Execution Environment for secure code execution             |
# | WFI/WFE              | Supported            | Instructions to wait for interrupt/event (low-power state)          |
# | ECC                  | Only CSRs Supported  | Error Correction Code for detecting/correcting memory errors        |
# | CCM                  | Only CSRs Supported  | Coherence Support for Cache/Memory                                  |
# | SPMP                 | Not supported        | Supervisor Physical Memory Protection for privileged isolation      |
# | SMP&CLUSTER CACHE    | Supported            | Manages cache coherency across multiple cores/clusters              |
# | UART                 | Supported            | Serial communication via Universal Asynchronous Receiver/Transmitter|
# | GPIO                 | Supported            | General Purpose Input/Output for controlling peripherals            |
# | QSPI                 | Supported            | Quad SPI interface for high-speed flash memory access               |
# | TEST FINISHER        | Supported            | Mechanism to signal the end of a test or simulation                 |
# +----------------------+----------------------+---------------------------------------------------------------------+
# 
# Unified QEMU+GDB Runner Script

# Configuration
ELF_FILE="../build/ms1.elf"  # Your compiled program
GDB_PORT=1234       # GDB-QEMU communication port

# Get project root directory
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

# Set toolchain paths
GCC_DIR="$ROOT_DIR/toolchain/gcc/bin"
QEMU_DIR="$ROOT_DIR/toolchain/qemu/bin"

# Update PATH environment variable
export PATH="$GCC_DIR:$QEMU_DIR:$PATH"

# Start QEMU in paused state (waiting for GDB connection)
# -machine: Target development board emulation
# -cpu: Specific CPU model to emulate
# -gdb: Enable GDB server on specified port
# -serial: Redirect virtual serial port to terminal
# -S: Pause CPU execution at startup
# qemu-system-riscv32 \
#     -machine nuclei_evalsoc,download=ilm,soc-cfg=ms1.json \
#     -cpu nuclei-n203 \
#     -smp 1 \
#     -gdb tcp::$GDB_PORT \
#     -serial stdio \
#     -nodefaults \
#     -S \
#     --semihosting-config enable=off,target=native \
#     -device loader,file=$ELF_FILE,addr=0xf9000000&

qemu-system-riscv32 \
    -machine nuclei_evalsoc,download=ilm,soc-cfg=ms1.json \
    -cpu nuclei-n203 \
    -smp 1 \
    -gdb tcp::$GDB_PORT \
    -serial stdio \
    -nodefaults \
    -S \
    --semihosting-config enable=off,target=native \
    -device loader,file=$ELF_FILE,addr=0xf9000000&

# Store QEMU process ID for cleanup
QEMU_PID=$!

# Brief pause to ensure QEMU starts before GDB connects
sleep 1

echo "QEMU started with GDB server on port $GDB_PORT (PID: $QEMU_PID)."
echo "Press Ctrl+C to stop QEMU after debugging."

# Wait for user to terminate (keeps QEMU running until you stop it)
trap "kill $QEMU_PID; echo 'QEMU terminated.'" INT
wait $QEMU_PID

# # Automated GDB session
# # -ex: Execute GDB commands sequentially
# # target remote: Connect to QEMU instance
# # load: Flash the ELF file to memory
# # continue: Start program execution
# # detach: Release connection
# # quit: Exit GDB
# riscv64-unknown-elf-gdb \
#     -ex "target remote :$GDB_PORT" \
#     -ex "file $ELF_FILE" \
#     -ex "break freeloader.S:100" \
#     -ex "continue"
# #    -ex "break _start" \
# #    -ex "layout asm" \y
# 
# # Clean up QEMU process after GDB exits
# kill $QEMU_PID
# wait $QEMU_PID  # Wait for termination
# 
# echo "Execution sequence completed"
# 