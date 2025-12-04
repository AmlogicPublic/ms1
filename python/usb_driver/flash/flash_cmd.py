"""
W25Q32DW Flash编程驱动
基于SPI接口实现Flash的擦除、编程、读取功能
支持4MB (32Mbit) Flash存储器

支持模式:
- Standard SPI Mode (标准SPI模式)
- Dual SPI Mode (双SPI模式)  
- Quad SPI Mode (四SPI模式)
- QPI Mode (四线外设接口模式)
"""
import os
import time
from typing import List, Tuple, Optional
from spi_driver import *

spi_guard_time_us = 50

# ==================== W25Q32DW 设备识别信息 ====================


class DeviceInfo:
    """W25Q32DW设备识别信息"""
    MANUFACTURER_ID = 0xEF         # Winbond厂商ID
    DEVICE_ID_ABH = 0x15           # ABh指令返回的设备ID
    DEVICE_ID_90H = 0x6016         # 90h指令返回的设备ID (Memory Type + Capacity)
    MEMORY_TYPE = 0x60             # 存储器类型
    CAPACITY_ID = 0x16             # 容量标识 (32Mbit)

    # 预期的JEDEC ID组合 [厂商ID, 存储类型, 容量]
    EXPECTED_JEDEC_ID = [MANUFACTURER_ID, MEMORY_TYPE, CAPACITY_ID]

# ==================== Flash规格常量 ====================


class FlashSpecs:
    """W25Q32DW Flash规格参数"""
    FLASH_SIZE = 0x400000     # 总容量: 4MB (32Mbit)
    PAGE_SIZE = 256          # 页大小: 256字节
    SECTOR_SIZE = 4096         # 扇区大小: 4KB
    BLOCK_32K_SIZE = 32768        # 32KB块大小
    BLOCK_64K_SIZE = 65536        # 64KB块大小
    MIN_ADDRESS = 0x000000     # 最小地址
    MAX_ADDRESS = 0x3FFFFF     # 最大地址

# ==================== 状态寄存器位定义 ====================


class StatusRegister1Bits:
    """状态寄存器1位定义"""
    BUSY = 0x01      # Bit 0: 忙标志 (1=忙碌, 0=空闲)
    WEL = 0x02      # Bit 1: 写使能锁存 (1=使能, 0=禁止)
    BP0 = 0x04      # Bit 2: 块保护位0
    BP1 = 0x08      # Bit 3: 块保护位1
    BP2 = 0x10      # Bit 4: 块保护位2
    TB = 0x20      # Bit 5: 顶部/底部块保护选择
    SEC = 0x40      # Bit 6: 扇区保护
    SRP0 = 0x80      # Bit 7: 状态寄存器保护0


class StatusRegister2Bits:
    """状态寄存器2位定义"""
    SRP1 = 0x01      # Bit 8:  状态寄存器保护1
    QE = 0x02      # Bit 9:  四线使能 (1=使能四线模式)
    LB0 = 0x04      # Bit 10: 安全寄存器锁定位0 (OTP)
    LB1 = 0x08      # Bit 11: 安全寄存器锁定位1 (OTP)
    LB2 = 0x10      # Bit 12: 安全寄存器锁定位2 (OTP)
    LB3 = 0x20      # Bit 13: 安全寄存器锁定位3 (OTP)
    CMP = 0x40      # Bit 14: 补码保护
    SUS = 0x80      # Bit 15: 挂起状态 (1=挂起中)

# ==================== 全双工SPI指令定义系统 ====================


# 基础符号定义
def X(): return 0xFF        # Dummy byte - 发送0xFF


_ = None                # Ignore - 忽略该位置的接收数据


class SPIFullDuplexCmd:
    """全双工SPI指令定义类 - 精确控制每个字节的MOSI和MISO"""

    def __init__(self, name: str, bytes_def: List, description: str = ""):
        self.name = name
        self.description = description
        self.bytes_def = bytes_def  # [(mosi_byte, miso_field), ...]

    def get_mosi(self, **params) -> List[int]:
        """获取MOSI发送数据 - 使用列表推导式避免循环卡死"""
        def process_byte(mosi_byte):
            if callable(mosi_byte):
                return mosi_byte(**params)
            elif mosi_byte is None:
                return 0xFF  # 默认发送0xFF
            else:
                return mosi_byte

        # 使用列表推导式替代for循环
        return [process_byte(mosi_byte) for mosi_byte, _ in self.bytes_def]

    def parse_miso(self, raw_miso: List[int]) -> dict:
        """解析MISO接收数据，返回字段字典 - 使用安全的方法避免循环卡死"""
        result = {}
        # 使用更安全的方式处理，避免enumerate导致的可能问题
        for i in range(len(self.bytes_def)):
            if i < len(raw_miso):
                _, field_name = self.bytes_def[i]
                if field_name is not None:
                    result[field_name] = raw_miso[i]
        return result

    def get_response_fields(self) -> List[str]:
        """获取所有响应字段名列表 - 使用安全的方法"""
        fields = []
        for i in range(len(self.bytes_def)):
            _, field = self.bytes_def[i]
            if field is not None:
                fields.append(field)
        return fields

    def get_total_bytes(self) -> int:
        """获取总字节数"""
        return len(self.bytes_def)

# ==================== 10.2.2 Instruction Set Table 1 (Standard SPI Instructions) ====================

# Write Enable


def WRITE_ENABLE() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("WRITE_ENABLE", [
        (0x06, _)  # BYTE 1: 06h
    ], "Write Enable")

# Volatile SR Write Enable


def VOLATILE_SR_WRITE_EN() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("VOLATILE_SR_WRITE_EN", [
        (0x50, _)  # BYTE 1: 50h
    ], "Volatile SR Write Enable")

# Write Disable


def WRITE_DISABLE() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("WRITE_DISABLE", [
        (0x04, _)  # BYTE 1: 04h
    ], "Write Disable")

# Read Status Register-1


def READ_STATUS1() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("READ_STATUS1", [
        (0x05, _),       # BYTE 1: 05h
        (X(), 'S7S0')    # BYTE 2: dummy → (S7-S0)
    ], "Read Status Register-1")

# Read Status Register-2


def READ_STATUS2() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("READ_STATUS2", [
        (0x35, _),        # BYTE 1: 35h
        (X(), 'S15S8')    # BYTE 2: dummy → (S15-S8)
    ], "Read Status Register-2")

# Write Status Register


def WRITE_STATUS_REG(s1: int, s2: int = None) -> SPIFullDuplexCmd:
    if s2 is not None:
        return SPIFullDuplexCmd("WRITE_STATUS_REG", [
            (0x01, _),  # BYTE 1: 01h
            (s1, _),    # BYTE 2: (S7-S0)
            (s2, _)     # BYTE 3: (S15-S8)
        ], "Write Status Register (2 bytes)")
    else:
        return SPIFullDuplexCmd("WRITE_STATUS_REG", [
            (0x01, _),  # BYTE 1: 01h
            (s1, _)     # BYTE 2: (S7-S0)
        ], "Write Status Register (1 byte)")

# Page Program


def PAGE_PROGRAM(addr: int, data: List[int]) -> SPIFullDuplexCmd:
    bytes_def = [
        (0x02, _),                    # BYTE 1: 02h
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A16
        ((addr >> 8) & 0xFF, _),      # BYTE 3: A15-A8
        (addr & 0xFF, _),             # BYTE 4: A7-A0
    ]
    # Add data bytes - 使用安全的方法避免循环卡死
    for i in range(len(data)):
        bytes_def.append((data[i], _))      # BYTE 5+: D7-D0, D7-D0, ...
    return SPIFullDuplexCmd("PAGE_PROGRAM", bytes_def, f"Page Program {len(data)} bytes")

# Sector Erase (4KB)


def SECTOR_ERASE_4KB(addr: int) -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("SECTOR_ERASE_4KB", [
        (0x20, _),                    # BYTE 1: 20h
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A16
        ((addr >> 8) & 0xFF, _),      # BYTE 3: A15-A8
        (addr & 0xFF, _),             # BYTE 4: A7-A0
    ], "Sector Erase (4KB)")

# Block Erase (32KB)


def BLOCK_ERASE_32KB(addr: int) -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("BLOCK_ERASE_32KB", [
        (0x52, _),                    # BYTE 1: 52h
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A16
        ((addr >> 8) & 0xFF, _),      # BYTE 3: A15-A8
        (addr & 0xFF, _),             # BYTE 4: A7-A0
    ], "Block Erase (32KB)")

# Block Erase (64KB)


def BLOCK_ERASE_64KB(addr: int) -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("BLOCK_ERASE_64KB", [
        (0xD8, _),                    # BYTE 1: D8h
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A16
        ((addr >> 8) & 0xFF, _),      # BYTE 3: A15-A8
        (addr & 0xFF, _),             # BYTE 4: A7-A0
    ], "Block Erase (64KB)")

# Chip Erase


def CHIP_ERASE() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("CHIP_ERASE", [
        (0xC7, _)  # BYTE 1: C7h
    ], "Chip Erase")

# Alternative Chip Erase


def CHIP_ERASE_ALT() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("CHIP_ERASE_ALT", [
        (0x60, _)  # BYTE 1: 60h
    ], "Chip Erase (Alternative)")

# Erase / Program Suspend


def ERASE_PROGRAM_SUSPEND() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("ERASE_PROGRAM_SUSPEND", [
        (0x75, _)  # BYTE 1: 75h
    ], "Erase / Program Suspend")

# Erase / Program Resume


def ERASE_PROGRAM_RESUME() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("ERASE_PROGRAM_RESUME", [
        (0x7A, _)  # BYTE 1: 7Ah
    ], "Erase / Program Resume")

# Power-down


def POWER_DOWN() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("POWER_DOWN", [
        (0xB9, _)  # BYTE 1: B9h
    ], "Power-down")

# Read Data


def READ_DATA(addr: int, length: int = 0) -> SPIFullDuplexCmd:
    bytes_def = [
        (0x03, _),                    # BYTE 1: 03h
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A16
        ((addr >> 8) & 0xFF, _),      # BYTE 3: A15-A8
        (addr & 0xFF, _),             # BYTE 4: A7-A0
    ]
    # Add data bytes if length specified
    for i in range(length):
        bytes_def.append((X(), f'D{i}'))  # BYTE 5+: (D7-D0), (D7-D0), ...
    return SPIFullDuplexCmd("READ_DATA", bytes_def, f"Read Data {length} bytes")

# Fast Read


def FAST_READ(addr: int, length: int = 0) -> SPIFullDuplexCmd:
    bytes_def = [
        (0x0B, _),                    # BYTE 1: 0Bh
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A16
        ((addr >> 8) & 0xFF, _),      # BYTE 3: A15-A8
        (addr & 0xFF, _),             # BYTE 4: A7-A0
        (X(), _),                     # BYTE 5: dummy
    ]
    # Add data bytes if length specified
    for i in range(length):
        bytes_def.append((X(), f'D{i}'))  # BYTE 6+: (D7-D0), (D7-D0), ...
    return SPIFullDuplexCmd("FAST_READ", bytes_def, f"Fast Read {length} bytes")

# Release Powerdown / ID


def RELEASE_POWERDOWN_ID() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("RELEASE_POWERDOWN_ID", [
        (0xAB, _),       # BYTE 1: ABh
        (X(), _),        # BYTE 2: dummy
        (X(), _),        # BYTE 3: dummy
        (X(), _),        # BYTE 4: dummy
        (X(), 'ID7ID0')  # BYTE 5: dummy → (ID7-ID0)
    ], "Release Powerdown / ID")

# Manufacturer/Device ID


def MANUFACTURER_DEVICE_ID() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("MANUFACTURER_DEVICE_ID", [
        (0x90, _),         # BYTE 1: 90h
        (X(), _),          # BYTE 2: dummy
        (X(), _),          # BYTE 3: dummy
        (0x00, _),         # BYTE 4: 00h
        (X(), 'MF7MF0'),   # BYTE 5: dummy → (MF7-MF0)
        (X(), 'ID7ID0')    # BYTE 6: dummy → (ID7-ID0)
    ], "Manufacturer/Device ID")

# JEDEC ID


def JEDEC_ID() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("JEDEC_ID", [
        (0x9F, _),            # BYTE 1: 9Fh
        (X(), 'MF7MF0'),      # BYTE 2: dummy → (MF7-MF0) Manufacturer
        (X(), 'ID15ID8'),     # BYTE 3: dummy → (ID15-ID8) Memory Type
        (X(), 'ID7ID0')       # BYTE 4: dummy → (ID7-ID0) Capacity
    ], "JEDEC ID")

# Read Unique ID


def READ_UNIQUE_ID(uid_bytes: int = 8) -> SPIFullDuplexCmd:
    bytes_def = [
        (0x4B, _),  # BYTE 1: 4Bh
        (X(), _),   # BYTE 2: dummy
        (X(), _),   # BYTE 3: dummy
        (X(), _),   # BYTE 4: dummy
        (X(), _),   # BYTE 5: dummy
    ]
    # Add UID bytes (UID63-UID0, typically 8 bytes)
    for i in range(uid_bytes):
        bytes_def.append((X(), f'UID{(uid_bytes-1-i)*8+7}UID{(uid_bytes-1-i)*8}'))
    return SPIFullDuplexCmd("READ_UNIQUE_ID", bytes_def, f"Read Unique ID {uid_bytes} bytes")

# Erase Security Registers


def ERASE_SECURITY_REG(reg_num: int, addr: int = 0x00) -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("ERASE_SECURITY_REG", [
        (0x44, _),                    # BYTE 1: 44h
        (0x00, _),                    # BYTE 2: A23-A16
        (reg_num * 0x10, _),          # BYTE 3: A15-A8
        (addr, _),                    # BYTE 4: A7-A0
    ], f"Erase Security Register {reg_num}")

# Program Security Registers


def PROGRAM_SECURITY_REG(reg_num: int, addr: int, data: List[int]) -> SPIFullDuplexCmd:
    bytes_def = [
        (0x42, _),                    # BYTE 1: 42h
        (0x00, _),                    # BYTE 2: A23-A16
        (reg_num * 0x10, _),          # BYTE 3: A15-A8
        (addr, _),                    # BYTE 4: A7-A0
    ]
    # Add data bytes - 使用安全的方法避免循环卡死
    for i in range(len(data)):
        bytes_def.append((data[i], _))      # BYTE 5+: D7-D0, D7-D0, ...
    return SPIFullDuplexCmd("PROGRAM_SECURITY_REG", bytes_def, f"Program Security Register {reg_num}")

# Read Security Registers


def READ_SECURITY_REG(reg_num: int, addr: int = 0x00, length: int = 0) -> SPIFullDuplexCmd:
    bytes_def = [
        (0x48, _),                    # BYTE 1: 48h
        (0x00, _),                    # BYTE 2: A23-A16
        (reg_num * 0x10, _),          # BYTE 3: A15-A8
        (addr, _),                    # BYTE 4: A7-A0
        (X(), _),                     # BYTE 5: dummy
    ]
    # Add data bytes if length specified
    for i in range(length):
        bytes_def.append((X(), f'D{i}'))  # BYTE 6+: (D7-D0), (D7-D0), ...
    return SPIFullDuplexCmd("READ_SECURITY_REG", bytes_def, f"Read Security Register {reg_num}")

# Enable QPI


def ENABLE_QPI() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("ENABLE_QPI", [
        (0x38, _)  # BYTE 1: 38h
    ], "Enable QPI")

# Enable Reset


def ENABLE_RESET() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("ENABLE_RESET", [
        (0x66, _)  # BYTE 1: 66h
    ], "Enable Reset")

# Reset


def RESET_DEVICE() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("RESET_DEVICE", [
        (0x99, _)  # BYTE 1: 99h
    ], "Reset")

# ==================== 10.2.3 Instruction Set Table 2 (Dual SPI Instructions) ====================

# Fast Read Dual Output


def FAST_READ_DUAL_OUTPUT(addr: int, length: int = 0) -> SPIFullDuplexCmd:
    bytes_def = [
        (0x3B, _),                    # BYTE 1: 3Bh
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A16
        ((addr >> 8) & 0xFF, _),      # BYTE 3: A15-A8
        (addr & 0xFF, _),             # BYTE 4: A7-A0
        (X(), _),                     # BYTE 5: dummy
    ]
    # Add dual data bytes if length specified (dual output format)
    for i in range(length):
        bytes_def.append((X(), f'DD{i}'))  # BYTE 6+: (D7-D0,…), dual output
    return SPIFullDuplexCmd("FAST_READ_DUAL_OUTPUT", bytes_def, f"Fast Read Dual Output {length} bytes")

# Fast Read Dual I/O


def FAST_READ_DUAL_IO(addr: int, mode: int = 0x00, length: int = 0) -> SPIFullDuplexCmd:
    bytes_def = [
        (0xBB, _),                    # BYTE 1: BBh
        # Note: A23-A8 and A7-A0,M7-M0 are sent in dual format, simplified here
        ((addr >> 8) & 0xFFFF, _),    # BYTE 2-3: A23-A8 (dual format)
        ((addr & 0xFF) | (mode << 8), _),  # BYTE 4: A7-A0,M7-M0 (dual format)
    ]
    # Add dual I/O data bytes if length specified
    for i in range(length):
        bytes_def.append((X(), f'DI{i}'))  # BYTE 4+: (D7-D0,…), dual I/O
    return SPIFullDuplexCmd("FAST_READ_DUAL_IO", bytes_def, f"Fast Read Dual I/O {length} bytes")

# Manufacturer/Device ID by Dual I/O


def MANUFACTURER_ID_DUAL_IO(addr: int = 0x00, mode: int = 0x00) -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("MANUFACTURER_ID_DUAL_IO", [
        (0x92, _),                    # BYTE 1: 92h
        # Note: Address and mode in dual format, simplified
        ((addr >> 8) & 0xFFFF, _),    # BYTE 2-3: A23-A8 (dual format)
        ((addr & 0xFF) | (mode << 8), _),  # BYTE 4: A7-A0,M7-M0 (dual format)
        (X(), 'MF7MF0'),              # BYTE 5: (MF7-MF0,ID7-ID0), dual I/O
        (X(), 'ID7ID0'),
    ], "Manufacturer/Device ID by Dual I/O")

# ==================== 10.2.4 Instruction Set Table 3 (Quad SPI Instructions) ====================

# Quad Page Program


def QUAD_PAGE_PROGRAM(addr: int, data: List[int]) -> SPIFullDuplexCmd:
    bytes_def = [
        (0x32, _),                    # BYTE 1: 32h
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A16
        ((addr >> 8) & 0xFF, _),      # BYTE 3: A15-A8
        (addr & 0xFF, _),             # BYTE 4: A7-A0
    ]
    # Add quad data bytes - 使用安全的方法避免循环卡死
    for i in range(len(data)):
        bytes_def.append((data[i], _))      # BYTE 5+: D7-D0,… (quad format)
    return SPIFullDuplexCmd("QUAD_PAGE_PROGRAM", bytes_def, f"Quad Page Program {len(data)} bytes")

# Fast Read Quad Output


def FAST_READ_QUAD_OUTPUT(addr: int, length: int = 0) -> SPIFullDuplexCmd:
    bytes_def = [
        (0x6B, _),                    # BYTE 1: 6Bh
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A16
        ((addr >> 8) & 0xFF, _),      # BYTE 3: A15-A8
        (addr & 0xFF, _),             # BYTE 4: A7-A0
        (X(), _),                     # BYTE 5: dummy
    ]
    # Add quad output data bytes
    for i in range(length):
        bytes_def.append((X(), f'QD{i}'))  # BYTE 6+: (D7-D0,…), quad output
    return SPIFullDuplexCmd("FAST_READ_QUAD_OUTPUT", bytes_def, f"Fast Read Quad Output {length} bytes")

# Fast Read Quad I/O


def FAST_READ_QUAD_IO(addr: int, mode: int = 0x00, length: int = 0) -> SPIFullDuplexCmd:
    bytes_def = [
        (0xEB, _),                    # BYTE 1: EBh
        # Note: A23-A0,M7-M0 in quad format, simplified
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A0,M7-M0 (quad format, part 1)
        ((addr >> 8) & 0xFF, _),      # BYTE 3: (quad format, part 2)
        ((addr & 0xFF) | (mode << 8), _),  # BYTE 4: (quad format, part 3)
        (X(), _),                     # BYTE 5: (xxxx,D7-D0), quad format
    ]
    # Add quad I/O data bytes
    for i in range(length):
        bytes_def.append((X(), f'QI{i}'))  # BYTE 6+: (D7-D0,…), quad I/O
    return SPIFullDuplexCmd("FAST_READ_QUAD_IO", bytes_def, f"Fast Read Quad I/O {length} bytes")

# Word Read Quad I/O


def WORD_READ_QUAD_IO(addr: int, mode: int = 0x00, length: int = 0) -> SPIFullDuplexCmd:
    bytes_def = [
        (0xE7, _),                    # BYTE 1: E7h
        # Note: A23-A0,M7-M0 in quad format
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A0,M7-M0 (quad format)
        ((addr >> 8) & 0xFF, _),      # BYTE 3: (quad format)
        ((addr & 0xFF) | (mode << 8), _),  # BYTE 4: (quad format)
        (X(), _),                     # BYTE 5: (xx,D7-D0), quad format
    ]
    # Add quad I/O data bytes
    for i in range(length):
        bytes_def.append((X(), f'WQ{i}'))  # BYTE 6+: (D7-D0,…), word quad I/O
    return SPIFullDuplexCmd("WORD_READ_QUAD_IO", bytes_def, f"Word Read Quad I/O {length} bytes")

# Octal Word Read Quad I/O


def OCTAL_WORD_READ_QUAD_IO(addr: int, mode: int = 0x00, length: int = 0) -> SPIFullDuplexCmd:
    bytes_def = [
        (0xE3, _),                    # BYTE 1: E3h
        # Note: A23-A0,M7-M0 in quad format
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A0,M7-M0 (quad format)
        ((addr >> 8) & 0xFF, _),      # BYTE 3: (quad format)
        ((addr & 0xFF) | (mode << 8), _),  # BYTE 4: (quad format)
    ]
    # Add quad I/O data bytes (no dummy for octal word read)
    for i in range(length):
        bytes_def.append((X(), f'OQ{i}'))  # BYTE 5+: (D7-D0,…), octal word quad I/O
    return SPIFullDuplexCmd("OCTAL_WORD_READ_QUAD_IO", bytes_def, f"Octal Word Read Quad I/O {length} bytes")

# Set Burst with Wrap


def SET_BURST_WITH_WRAP(wrap_bits: int = 0x00) -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("SET_BURST_WITH_WRAP", [
        (0x77, _),        # BYTE 1: 77h
        (0x00, _),        # BYTE 2: xxxxxx (reserved)
        (0x00, _),        # BYTE 3: xxxxxx (reserved)
        (0x00, _),        # BYTE 4: xxxxxx (reserved)
        (wrap_bits, _),   # BYTE 5: W6-W4 (wrap bits)
    ], "Set Burst with Wrap")

# Manufacture/Device ID by Quad I/O


def MANUFACTURER_ID_QUAD_IO(addr: int = 0x00, mode: int = 0x00) -> SPIFullDuplexCmd:
    bytes_def = [
        (0x94, _),                    # BYTE 1: 94h
        # Note: A23-A0,M7-M0 in quad format
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A0,M7-M0 (quad format)
        ((addr >> 8) & 0xFF, _),      # BYTE 3: (quad format)
        ((addr & 0xFF) | (mode << 8), _),  # BYTE 4: (quad format)
        (X(), 'xxxx'),                # BYTE 5: xxxx,(MF7-MF0,ID7-ID0), quad I/O
        (X(), 'MF7MF0'),              # BYTE 6: (MF7-MF0,ID7-ID0,…), continuous
        (X(), 'ID7ID0'),              # BYTE 7: continuation
    ]
    return SPIFullDuplexCmd("MANUFACTURER_ID_QUAD_IO", bytes_def, "Manufacture/Device ID by Quad I/O")

# ==================== 10.2.5 Instruction Set Table 4 (QPI Instructions) ====================
# Note: QPI instructions use 4-bit data on all signals (Command, Address, Data)

# QPI Write Enable


def QPI_WRITE_ENABLE() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_WRITE_ENABLE", [
        (0x06, _)  # BYTE 1: 06h (QPI)
    ], "Write Enable (QPI)")

# QPI Volatile SR Write Enable


def QPI_VOLATILE_SR_WRITE_EN() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_VOLATILE_SR_WRITE_EN", [
        (0x50, _)  # BYTE 1: 50h (QPI)
    ], "Volatile SR Write Enable (QPI)")

# QPI Write Disable


def QPI_WRITE_DISABLE() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_WRITE_DISABLE", [
        (0x04, _)  # BYTE 1: 04h (QPI)
    ], "Write Disable (QPI)")

# QPI Read Status Register-1


def QPI_READ_STATUS1() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_READ_STATUS1", [
        (0x05, _),       # BYTE 1: 05h (QPI)
        (X(), 'S7S0')    # BYTE 2: (S7-S0) (QPI)
    ], "Read Status Register-1 (QPI)")

# QPI Read Status Register-2


def QPI_READ_STATUS2() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_READ_STATUS2", [
        (0x35, _),        # BYTE 1: 35h (QPI)
        (X(), 'S15S8')    # BYTE 2: (S15-S8) (QPI)
    ], "Read Status Register-2 (QPI)")

# QPI Write Status Register


def QPI_WRITE_STATUS_REG(s1: int, s2: int = None) -> SPIFullDuplexCmd:
    if s2 is not None:
        return SPIFullDuplexCmd("QPI_WRITE_STATUS_REG", [
            (0x01, _),  # BYTE 1: 01h (QPI)
            (s1, _),    # BYTE 2: (S7-S0) (QPI)
            (s2, _)     # BYTE 3: (S15-S8) (QPI)
        ], "Write Status Register (QPI, 2 bytes)")
    else:
        return SPIFullDuplexCmd("QPI_WRITE_STATUS_REG", [
            (0x01, _),  # BYTE 1: 01h (QPI)
            (s1, _)     # BYTE 2: (S7-S0) (QPI)
        ], "Write Status Register (QPI, 1 byte)")

# QPI Page Program


def QPI_PAGE_PROGRAM(addr: int, data: List[int]) -> SPIFullDuplexCmd:
    bytes_def = [
        (0x02, _),                    # BYTE 1: 02h (QPI)
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A16 (QPI)
        ((addr >> 8) & 0xFF, _),      # BYTE 3: A15-A8 (QPI)
        (addr & 0xFF, _),             # BYTE 4: A7-A0 (QPI)
    ]
    # Add data bytes (QPI mode) - 使用安全的方法避免循环卡死
    for i in range(len(data)):
        bytes_def.append((data[i], _))      # BYTE 5+: D7-D0 (QPI)
    return SPIFullDuplexCmd("QPI_PAGE_PROGRAM", bytes_def, f"Page Program (QPI, {len(data)} bytes)")

# QPI Sector Erase (4KB)


def QPI_SECTOR_ERASE_4KB(addr: int) -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_SECTOR_ERASE_4KB", [
        (0x20, _),                    # BYTE 1: 20h (QPI)
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A16 (QPI)
        ((addr >> 8) & 0xFF, _),      # BYTE 3: A15-A8 (QPI)
        (addr & 0xFF, _),             # BYTE 4: A7-A0 (QPI)
    ], "Sector Erase 4KB (QPI)")

# QPI Block Erase (32KB)


def QPI_BLOCK_ERASE_32KB(addr: int) -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_BLOCK_ERASE_32KB", [
        (0x52, _),                    # BYTE 1: 52h (QPI)
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A16 (QPI)
        ((addr >> 8) & 0xFF, _),      # BYTE 3: A15-A8 (QPI)
        (addr & 0xFF, _),             # BYTE 4: A7-A0 (QPI)
    ], "Block Erase 32KB (QPI)")

# QPI Block Erase (64KB)


def QPI_BLOCK_ERASE_64KB(addr: int) -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_BLOCK_ERASE_64KB", [
        (0xD8, _),                    # BYTE 1: D8h (QPI)
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A16 (QPI)
        ((addr >> 8) & 0xFF, _),      # BYTE 3: A15-A8 (QPI)
        (addr & 0xFF, _),             # BYTE 4: A7-A0 (QPI)
    ], "Block Erase 64KB (QPI)")

# QPI Chip Erase


def QPI_CHIP_ERASE() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_CHIP_ERASE", [
        (0xC7, _)  # BYTE 1: C7h/60h (QPI)
    ], "Chip Erase (QPI)")

# QPI Erase / Program Suspend


def QPI_ERASE_PROGRAM_SUSPEND() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_ERASE_PROGRAM_SUSPEND", [
        (0x75, _)  # BYTE 1: 75h (QPI)
    ], "Erase / Program Suspend (QPI)")

# QPI Erase / Program Resume


def QPI_ERASE_PROGRAM_RESUME() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_ERASE_PROGRAM_RESUME", [
        (0x7A, _)  # BYTE 1: 7Ah (QPI)
    ], "Erase / Program Resume (QPI)")

# QPI Power-down


def QPI_POWER_DOWN() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_POWER_DOWN", [
        (0xB9, _)  # BYTE 1: B9h (QPI)
    ], "Power-down (QPI)")

# Set Read Parameters


def SET_READ_PARAMETERS(params: int) -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("SET_READ_PARAMETERS", [
        (0xC0, _),    # BYTE 1: C0h
        (params, _),  # BYTE 2: P7-P0
    ], "Set Read Parameters")

# QPI Fast Read


def QPI_FAST_READ(addr: int, dummy_len: int = 0, length: int = 0) -> SPIFullDuplexCmd:
    bytes_def = [
        (0x0B, _),                    # BYTE 1: 0Bh (QPI)
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A16 (QPI)
        ((addr >> 8) & 0xFF, _),      # BYTE 3: A15-A8 (QPI)
        (addr & 0xFF, _),             # BYTE 4: A7-A0 (QPI)
    ]
    # Add dummy bytes
    for i in range(dummy_len):
        bytes_def.append((X(), _))    # Dummy bytes
    # Add data bytes
    for i in range(length):
        bytes_def.append((X(), f'QFD{i}'))  # BYTE 5+dummy+: (D7-D0) (QPI)
    return SPIFullDuplexCmd("QPI_FAST_READ", bytes_def, f"Fast Read (QPI, {length} bytes)")

# QPI Burst Read with Wrap


def QPI_BURST_READ_WRAP(addr: int, dummy_len: int = 0, length: int = 0) -> SPIFullDuplexCmd:
    bytes_def = [
        (0x0C, _),                    # BYTE 1: 0Ch (QPI)
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A16 (QPI)
        ((addr >> 8) & 0xFF, _),      # BYTE 3: A15-A8 (QPI)
        (addr & 0xFF, _),             # BYTE 4: A7-A0 (QPI)
    ]
    # Add dummy bytes
    for i in range(dummy_len):
        bytes_def.append((X(), _))    # Dummy bytes
    # Add data bytes
    for i in range(length):
        bytes_def.append((X(), f'QBD{i}'))  # BYTE 5+dummy+: (D7-D0) (QPI)
    return SPIFullDuplexCmd("QPI_BURST_READ_WRAP", bytes_def, f"Burst Read with Wrap (QPI, {length} bytes)")

# QPI Fast Read Quad I/O


def QPI_FAST_READ_QUAD_IO(addr: int, mode: int = 0x00, dummy_len: int = 0, length: int = 0) -> SPIFullDuplexCmd:
    bytes_def = [
        (0xEB, _),                    # BYTE 1: EBh (QPI)
        ((addr >> 16) & 0xFF, _),     # BYTE 2: A23-A16 (QPI)
        ((addr >> 8) & 0xFF, _),      # BYTE 3: A15-A8 (QPI)
        (addr & 0xFF, _),             # BYTE 4: A7-A0 (QPI)
        (mode, _),                    # BYTE 5: M7-M0 (QPI)
    ]
    # Add dummy bytes
    for i in range(dummy_len):
        bytes_def.append((X(), _))    # Dummy bytes
    # Add data bytes
    for i in range(length):
        bytes_def.append((X(), f'QQID{i}'))  # BYTE 6+dummy+: (D7-D0) (QPI)
    return SPIFullDuplexCmd("QPI_FAST_READ_QUAD_IO", bytes_def, f"Fast Read Quad I/O (QPI, {length} bytes)")

# QPI Release Powerdown / ID


def QPI_RELEASE_POWERDOWN_ID() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_RELEASE_POWERDOWN_ID", [
        (0xAB, _),       # BYTE 1: ABh (QPI)
        (X(), _),        # BYTE 2: dummy (QPI)
        (X(), _),        # BYTE 3: dummy (QPI)
        (X(), _),        # BYTE 4: dummy (QPI)
        (X(), 'ID7ID0')  # BYTE 5: (ID7-ID0) (QPI)
    ], "Release Powerdown / ID (QPI)")

# QPI Manufacturer/Device ID


def QPI_MANUFACTURER_DEVICE_ID() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_MANUFACTURER_DEVICE_ID", [
        (0x90, _),         # BYTE 1: 90h (QPI)
        (X(), _),          # BYTE 2: dummy (QPI)
        (X(), _),          # BYTE 3: dummy (QPI)
        (0x00, _),         # BYTE 4: 00h (QPI)
        (X(), 'MF7MF0'),   # BYTE 5: (MF7-MF0) (QPI)
        (X(), 'ID7ID0')    # BYTE 6: (ID7-ID0) (QPI)
    ], "Manufacturer/Device ID (QPI)")

# QPI JEDEC ID


def QPI_JEDEC_ID() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_JEDEC_ID", [
        (0x9F, _),            # BYTE 1: 9Fh (QPI)
        (X(), 'MF7MF0'),      # BYTE 2: (MF7-MF0) Manufacturer (QPI)
        (X(), 'ID15ID8'),     # BYTE 3: (ID15-ID8) Memory Type (QPI)
        (X(), 'ID7ID0')       # BYTE 4: (ID7-ID0) Capacity (QPI)
    ], "JEDEC ID (QPI)")

# Disable QPI


def DISABLE_QPI() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("DISABLE_QPI", [
        (0xFF, _)  # BYTE 1: FFh
    ], "Disable QPI")

# QPI Enable Reset


def QPI_ENABLE_RESET() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_ENABLE_RESET", [
        (0x66, _)  # BYTE 1: 66h (QPI)
    ], "Enable Reset (QPI)")

# QPI Reset Device


def QPI_RESET_DEVICE() -> SPIFullDuplexCmd:
    return SPIFullDuplexCmd("QPI_RESET_DEVICE", [
        (0x99, _)  # BYTE 1: 99h (QPI)
    ], "Reset (QPI)")

# ==================== 地址处理工具 ====================


class AddressUtils:
    """地址处理工具"""

    @staticmethod
    def align_to_page(address: int) -> int:
        """对齐到页边界"""
        return address & ~(FlashSpecs.PAGE_SIZE - 1)

    @staticmethod
    def align_to_sector(address: int) -> int:
        """对齐到扇区边界"""
        return address & ~(FlashSpecs.SECTOR_SIZE - 1)

    @staticmethod
    def align_to_block_64k(address: int) -> int:
        """对齐到64KB块边界"""
        return address & ~(FlashSpecs.BLOCK_64K_SIZE - 1)

    @staticmethod
    def align_to_block_32k(address: int) -> int:
        """对齐到32KB块边界"""
        return address & ~(FlashSpecs.BLOCK_32K_SIZE - 1)

    @staticmethod
    def is_valid_address(address: int) -> bool:
        """检查地址是否有效"""
        return FlashSpecs.MIN_ADDRESS <= address <= FlashSpecs.MAX_ADDRESS

    @staticmethod
    def is_valid_range(address: int, length: int) -> bool:
        """检查地址范围是否有效"""
        return (AddressUtils.is_valid_address(address) and
                AddressUtils.is_valid_address(address + length - 1))

    @staticmethod
    def get_page_boundary(address: int) -> int:
        """获取地址所在页的结束边界"""
        return AddressUtils.align_to_page(address) + FlashSpecs.PAGE_SIZE

    @staticmethod
    def will_cross_page_boundary(address: int, length: int) -> bool:
        """检查是否会跨越页边界"""
        return address + length > AddressUtils.get_page_boundary(address)

# ==================== W25Q32DW Flash 驱动程序 ====================


class W25Q32DWDriver:
    """W25Q32DW Flash驱动程序类 - 基于One-liner指令定义"""

    def __init__(self, spi_driver: SPIDriver, usb_index: int = 0, spi_mode: str = 'standard'):
        """
        初始化Flash驱动
        :param spi_driver: SPI驱动实例  
        :param usb_index: USB设备索引
        :param spi_mode: SPI模式 ('standard', 'dual', 'quad', 'qpi')
        """
        self.spi = spi_driver
        self.usb_index = usb_index
        self.spi_mode = spi_mode.lower()
        self.is_initialized = False

        # 设置SPI guard时间
        self.spi_guard_time_us = spi_guard_time_us

        # 验证SPI模式
        valid_modes = ['standard', 'dual', 'quad', 'qpi']
        if self.spi_mode not in valid_modes:
            raise ValueError(f"不支持的SPI模式: {spi_mode}，支持的模式: {valid_modes}")

    def initialize(self) -> bool:
        """
        初始化Flash设备
        :return: 初始化是否成功
        """
        # 配置SPI参数：低速开始，MSB优先，模式0
        if not self.spi.config_master_param(SPI_Rate_18M, SPI_MSB, SPI_SubMode_0, self.usb_index):
            print("✗ SPI参数配置失败")
            return False

        # 设置CS为高电平（空闲状态）
        if not self.spi.set_cs0(1, self.usb_index):
            print("✗ CS0设置失败")
            return False

        # 读取并验证JEDEC ID
        jedec_id = self.read_jedec_id()
        if not jedec_id or any(x is None for x in jedec_id):
            print("✗ JEDEC ID读取失败")
            return False

        manufacturer_id, memory_type, capacity_id = jedec_id

        # 验证设备ID
        if (manufacturer_id != DeviceInfo.MANUFACTURER_ID or
            capacity_id != DeviceInfo.CAPACITY_ID or
                memory_type not in [0x60, 0x40]):
            print(f"✗ 设备ID验证失败，期望厂商=0x{DeviceInfo.MANUFACTURER_ID:02X}")
            return False

        self.is_initialized = True
        return True

    # ==================== 底层SPI传输方法 ====================

    def _spi_guard_delay(self):
        """SPI命令间guard延迟，确保CS信号有足够时间重新拉高"""
        if self.spi_guard_time_us > 0:
            time.sleep(self.spi_guard_time_us / 1_000_000.0)  # 转换为秒

    def _send_command(self, cmd_bytes: List[int]) -> bool:
        """
        发送SPI命令 (仅发送，无接收)
        """
        result = self.spi.master_send_data(start_cs=0, end_cs=1, send_buf=cmd_bytes, usb_index=self.usb_index)
        self._spi_guard_delay()
        return result

    def _send_and_receive(self, cmd_bytes: List[int], read_len: int) -> Tuple[bool, List[int]]:
        """
        发送命令并接收数据
        """
        result = self.spi.master_send_and_rcv_data(
            cs_select=0, start_cs=0, end_cs=1, cs_delay=0,
            duplex=1, dummy=0xFF, send_buf=cmd_bytes,
            rcv_len=read_len, usb_index=self.usb_index
        )
        self._spi_guard_delay()
        return result

    def _execute_full_duplex_cmd(self, cmd: SPIFullDuplexCmd) -> Tuple[bool, dict]:
        """
        执行全双工SPI指令
        """
        mosi_data = cmd.get_mosi()
        total_bytes = cmd.get_total_bytes()
        success, raw_miso = self._send_and_receive(mosi_data, total_bytes)

        if success:
            return True, cmd.parse_miso(raw_miso)
        return False, {}

    # ==================== 数据提取工具方法 ====================

    def _extract_data_fields(self, response: dict, length: int, field_prefix: str = 'D') -> List[int]:
        """
        统一的数据字段提取方法 - 优雅的映射实现
        :param response: SPI响应字典
        :param length: 需要提取的字段数量
        :param field_prefix: 字段前缀，默认为'D'
        :return: 提取的数据列表
        """
        # 使用简单循环方法，避免map函数的潜在问题
        result = []
        for i in range(length):
            field_value = response.get(f'{field_prefix}{i}', 0)
            result.append(field_value)
        return result

    # ==================== 基础Flash操作 (使用全双工指令系统) ====================

    def read_jedec_id(self) -> Optional[List[int]]:
        """读取JEDEC ID"""
        success, response = self._execute_full_duplex_cmd(JEDEC_ID())
        if success:
            return [response.get('MF7MF0'), response.get('ID15ID8'), response.get('ID7ID0')]
        return None

    def read_status_register1(self) -> Optional[int]:
        """读取状态寄存器1"""
        success, response = self._execute_full_duplex_cmd(READ_STATUS1())
        return response.get('S7S0') if success else None

    def read_status_register2(self) -> Optional[int]:
        """读取状态寄存器2"""
        success, response = self._execute_full_duplex_cmd(READ_STATUS2())
        return response.get('S15S8') if success else None

    def is_busy(self) -> bool:
        """检查Flash是否忙碌"""
        status = self.read_status_register1()
        if status is not None:
            return bool(status & StatusRegister1Bits.BUSY)
        return True

    def wait_not_busy(self, timeout: float = 30.0) -> bool:
        """
        等待Flash操作完成
        """
        start_time = time.time()
        check_interval = 0.001 if timeout <= 5.0 else 0.01

        while time.time() - start_time < timeout:
            if not self.is_busy():
                return True
            time.sleep(check_interval)
        return False

    def write_enable(self, retries: int = 10, delay: float = 0.01) -> bool:
        """发送写使能命令并确认 WEL=1"""
        if not self._send_command(WRITE_ENABLE().get_mosi()):
            print("✗ 写使能命令发送失败")
            return False
        for _ in range(retries):
            status = self.read_status_register1()
            if status is not None and (status & StatusRegister1Bits.WEL):
                return True
            time.sleep(delay)
        print(f"✗ 写使能确认失败，最终状态: 0x{status:02X}" if status is not None else "✗ 写使能确认失败，无法读取状态")
        return False

    def write_disable(self, retries: int = 10, delay: float = 0.01) -> bool:
        """发送写禁止命令并确认 WEL=0"""
        if not self._send_command(WRITE_DISABLE().get_mosi()):
            print("✗ 写禁止命令发送失败")
            return False
        for _ in range(retries):
            status = self.read_status_register1()
            if status is not None and not (status & StatusRegister1Bits.WEL):
                return True
            time.sleep(delay)
        print("✗ 写禁止命令确认失败，最终状态: 0x{status:02X}" if status is not None else "✗ 写禁止命令确认失败，无法读取状态")
        return False

    # ==================== 读取操作 ====================

    def read_data(self, address: int, length: int) -> Tuple[bool, List[int]]:
        """
        读取Flash数据
        :param address: 起始地址
        :param length: 读取长度
        :return: (成功标志, 数据列表)
        """
        assert AddressUtils.is_valid_address(address), "地址超出范围"
        assert AddressUtils.is_valid_range(address, length), "读取范围超出Flash大小"

        # 等待Flash空闲
        if not self.wait_not_busy():
            print("✗ Flash忙碌，无法读取")
            return False, []

        # 使用全双工指令
        read_cmd = READ_DATA(address, length)
        success, response = self._execute_full_duplex_cmd(read_cmd)
        if success:
            # 使用统一的数据字段提取方法
            data = self._extract_data_fields(response, length)
            return True, data
        else:
            print("✗ SPI读取命令执行失败")
        return False, []

    def fast_read_data(self, address: int, length: int) -> Tuple[bool, List[int]]:
        """
        快速读取Flash数据
        :param address: 起始地址  
        :param length: 读取长度
        :return: (成功标志, 数据列表)
        """
        assert AddressUtils.is_valid_address(address), "地址超出范围"
        assert AddressUtils.is_valid_range(address, length), "读取范围超出Flash大小"

        # 等待Flash空闲
        if not self.wait_not_busy():
            return False, []

        # 使用全双工指令
        fast_read_cmd = FAST_READ(address, length)
        success, response = self._execute_full_duplex_cmd(fast_read_cmd)

        if success:
            # 使用统一的数据字段提取方法
            data = self._extract_data_fields(response, length)
            return True, data
        return False, []

    # ==================== 擦除操作 ====================

    def sector_erase(self, address: int) -> bool:
        """
        4KB扇区擦除
        :param address: 扇区地址(任意地址，会对齐到扇区边界)
        :return: 擦除是否成功
        """
        assert AddressUtils.is_valid_address(address), "地址超出范围"

        # 对齐到扇区边界
        sector_addr = AddressUtils.align_to_sector(address)
        print(f"擦除4KB扇区: 0x{sector_addr:06X}")

        # 等待Flash空闲
        if not self.wait_not_busy():
            print("✗ Flash忙碌，擦除失败")
            return False

        # 写使能
        if not self.write_enable():
            print("✗ 写使能失败")
            return False

        # 发送扇区擦除命令
        erase_cmd = SECTOR_ERASE_4KB(sector_addr)
        mosi_data = erase_cmd.get_mosi()
        if not self._send_command(mosi_data):
            print("✗ 扇区擦除命令发送失败")
            return False

        # 等待擦除完成
        if not self.wait_not_busy(timeout=2.0):  # 扇区擦除通常<400ms
            print("✗ 扇区擦除超时")
            return False

        print("✓ 4KB扇区擦除完成")
        return True

    def block_erase_32k(self, address: int) -> bool:
        """
        32KB块擦除
        :param address: 块地址(任意地址，会对齐到块边界) 
        :return: 擦除是否成功
        """
        assert AddressUtils.is_valid_address(address), "地址超出范围"

        # 对齐到32KB块边界
        block_addr = AddressUtils.align_to_block_32k(address)
        print(f"擦除32KB块: 0x{block_addr:06X}")

        # 等待Flash空闲
        if not self.wait_not_busy():
            print("✗ Flash忙碌，擦除失败")
            return False

        # 写使能
        if not self.write_enable():
            print("✗ 写使能失败")
            return False

        # 发送32KB块擦除命令
        erase_cmd = BLOCK_ERASE_32KB(block_addr)
        mosi_data = erase_cmd.get_mosi()
        if not self._send_command(mosi_data):
            print("✗ 32KB块擦除命令发送失败")
            return False

        # 等待擦除完成
        if not self.wait_not_busy(timeout=3.0):  # 32KB块擦除通常<1.5s
            print("✗ 32KB块擦除超时")
            return False

        print("✓ 32KB块擦除完成")
        return True

    def block_erase_64k(self, address: int) -> bool:
        """
        64KB块擦除
        :param address: 块地址(任意地址，会对齐到块边界) 
        :return: 擦除是否成功
        """
        assert AddressUtils.is_valid_address(address), "地址超出范围"

        # 对齐到64KB块边界
        block_addr = AddressUtils.align_to_block_64k(address)
        print(f"擦除64KB块: 0x{block_addr:06X}")

        # 等待Flash空闲
        if not self.wait_not_busy():
            print("✗ Flash忙碌，擦除失败")
            return False

        # 写使能
        if not self.write_enable():
            print("✗ 写使能失败")
            return False

        # 发送64KB块擦除命令
        erase_cmd = BLOCK_ERASE_64KB(block_addr)
        mosi_data = erase_cmd.get_mosi()
        if not self._send_command(mosi_data):
            print("✗ 64KB块擦除命令发送失败")
            return False

        # 等待擦除完成
        if not self.wait_not_busy(timeout=5.0):  # 64KB块擦除通常<2s
            print("✗ 64KB块擦除超时")
            return False

        print("✓ 64KB块擦除完成")
        return True

    def chip_erase(self) -> bool:
        """
        整片擦除
        :return: 擦除是否成功
        """
        print("开始整片擦除...")

        # 等待Flash空闲
        if not self.wait_not_busy():
            print("✗ Flash忙碌，擦除失败")
            return False

        # 写使能
        if not self.write_enable():
            print("✗ 写使能失败")
            return False

        # 发送整片擦除命令
        mosi_data = CHIP_ERASE().get_mosi()
        if not self._send_command(mosi_data):
            print("✗ 整片擦除命令发送失败")
            return False

        print("整片擦除进行中，请等待...")

        # 等待擦除完成 (可能需要很长时间)
        if not self.wait_not_busy(timeout=60.0):  # 整片擦除可能需要数十秒
            print("✗ 整片擦除超时")
            return False

        print("✓ 整片擦除完成")
        return True

    # ==================== 编程操作 ====================

    def page_program(self, address: int, data: List[int]) -> bool:
        """
        页编程
        :param address: 起始地址
        :param data: 要编程的数据(最多256字节)
        :return: 编程是否成功
        """
        assert AddressUtils.is_valid_address(address), "地址超出范围"
        assert 1 <= len(data) <= FlashSpecs.PAGE_SIZE, f"数据长度必须在1-{FlashSpecs.PAGE_SIZE}字节范围内"
        assert AddressUtils.is_valid_range(address, len(data)), "编程数据超出Flash范围"

        # 检查是否跨页
        if AddressUtils.will_cross_page_boundary(address, len(data)):
            print(f"⚠️ 警告: 跨页编程会导致地址回绕")

        # 等待Flash空闲
        if not self.wait_not_busy():
            print("✗ Flash忙碌，编程失败")
            return False

        # 写使能
        if not self.write_enable():
            print("✗ 写使能失败")
            return False
        # 发送页编程命令
        program_cmd = PAGE_PROGRAM(address, data)
        mosi_data = program_cmd.get_mosi()
        if not self._send_command(mosi_data):
            print("✗ 页编程命令发送失败")
            return False

        # 等待编程完成
        if not self.wait_not_busy(timeout=1.0):
            print("✗ 页编程超时")
            return False

        # 验证写使能位已清除
        final_status = self.read_status_register1()
        if final_status is not None and (final_status & StatusRegister1Bits.WEL):
            print(f"✗ 页编程后WEL位仍为1，状态: 0x{final_status:02X}")
            return False

        return True

    def program_data(self, address: int, data: List[int], verify: bool = True) -> bool:
        """
        编程数据(自动分页)
        :param address: 起始地址
        :param data: 要编程的数据
        :param verify: 是否校验
        :return: 编程是否成功
        """
        assert AddressUtils.is_valid_address(address), "地址超出范围"
        assert AddressUtils.is_valid_range(address, len(data)), "编程数据超出Flash范围"

        print(f"编程数据: 地址=0x{address:06X}, 长度={len(data)}字节")

        total_programmed = 0
        current_addr = address

        # 使用安全的分页编程循环，避免while循环卡死
        # 考虑起始地址可能不对齐页边界，需要更精确的页数计算
        first_page_remaining = AddressUtils.get_page_boundary(address) - address
        remaining_after_first = max(0, len(data) - first_page_remaining)
        max_pages = 1 + (remaining_after_first + FlashSpecs.PAGE_SIZE - 1) // FlashSpecs.PAGE_SIZE

        # print(f"  分页计算: 起始地址=0x{address:06X}, 数据长度={len(data)}, 预计需要{max_pages}页")

        for page_index in range(max_pages):
            if total_programmed >= len(data):
                break  # 所有数据已编程完成

            # 计算当前页内剩余空间
            page_boundary = AddressUtils.get_page_boundary(current_addr)
            remaining_in_page = page_boundary - current_addr

            # 确定本次编程长度
            chunk_size = min(remaining_in_page, len(data) - total_programmed)
            chunk_data = data[total_programmed:total_programmed + chunk_size]

            print(f"  编程页: 0x{current_addr:06X}, 长度={chunk_size}字节")

            # 执行页编程
            if not self.page_program(current_addr, chunk_data):
                print(f"✗ 页编程失败，地址: 0x{current_addr:06X}")
                return False

            total_programmed += chunk_size
            current_addr += chunk_size

        print("✓ 数据编程完成")

        # 校验数据
        if verify:
            print("开始校验数据...")
            success, read_data = self.read_data(address, len(data))

            if not success:
                print("✗ 读取数据失败，无法校验")
                return False

            if read_data != data:
                print("✗ 数据校验失败")
                print(f"  期望数据长度: {len(data)}, 实际数据长度: {len(read_data)}")
                print(f"  期望数据前8字节: {data[:8]}")
                print(f"  实际数据前8字节: {read_data[:8]}")

                # 遍历所有字节找到不匹配的位置
                error_count = 0
                max_len = min(len(data), len(read_data))

                for i in range(max_len):
                    expected = data[i]
                    actual = read_data[i]
                    if expected != actual:
                        print(f"  地址0x{address + i:06X}: 期望=0x{expected:02X}, 实际=0x{actual:02X}")
                        error_count += 1
                        if error_count >= 10:  # 只显示前10个错误
                            print("  ...")
                            break

                # 如果数据长度不同
                if len(data) != len(read_data):
                    print(f"  数据长度不匹配: 期望{len(data)}, 实际{len(read_data)}")

                # 如果没有找到不匹配的字节，尝试用列表比较找出问题
                if error_count == 0 and len(data) == len(read_data):
                    print("  奇怪：逐字节检查都匹配，但列表比较失败")
                    print("  检查数据类型:")
                    print(f"    期望数据类型: {type(data[0]) if len(data) > 0 else 'Empty'}")
                    print(f"    实际数据类型: {type(read_data[0]) if len(read_data) > 0 else 'Empty'}")

                    # 检查中间某些位置的数据
                    check_positions = [0, len(data)//4, len(data)//2, len(data)*3//4, len(data)-1] if len(data) > 0 else []
                    for pos in check_positions:
                        if pos < len(data) and pos < len(read_data):
                            print(f"    位置{pos}: 期望={data[pos]}({type(data[pos])}), 实际={read_data[pos]}({type(read_data[pos])})")

                return False

            print("✓ 数据校验通过")

        return True

    # ==================== 设备信息 ====================

    def get_device_info(self) -> dict:
        """
        获取设备信息
        :return: 设备信息字典
        """
        jedec_id = self.read_jedec_id()
        status1 = self.read_status_register1()
        status2 = self.read_status_register2()

        return {
            "jedec_id": [hex(x) for x in jedec_id] if jedec_id and all(x is not None for x in jedec_id) else None,
            "page_size": f"{FlashSpecs.PAGE_SIZE}B",
            "sector_size": f"{FlashSpecs.SECTOR_SIZE // 1024}KB",
            "block_32k_size": f"{FlashSpecs.BLOCK_32K_SIZE // 1024}KB",
            "block_64k_size": f"{FlashSpecs.BLOCK_64K_SIZE // 1024}KB",
            "capacity": f"{FlashSpecs.FLASH_SIZE // 1024 // 1024}MB",
            "status_register1": f"0x{status1:02X}" if status1 is not None else None,
            "status_register2": f"0x{status2:02X}" if status2 is not None else None,
            "is_busy": self.is_busy(),
            "write_enabled": bool(status1 & StatusRegister1Bits.WEL) if status1 is not None else None,
            "spi_mode": self.spi_mode
        }

# ==================== 测试和示例代码 ====================


def main():
    """Flash驱动测试主函数"""
    print("W25Q32DW Flash驱动测试 - 基于One-liner指令")
    print("=" * 60)

    # 初始化SPI驱动
    spi_driver = SPIDriver()

    # 加载DLL文件
    dll_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "USB2UARTSPIIICDLL.dll")
    spi_driver.load_dll(dll_path)
    print(f"\n✓ DLL加载成功: {dll_path}")

    # 打开USB设备
    if not spi_driver.open_usb(0):
        print("✗ 无法打开USB设备")
        return

    # 创建Flash驱动实例 (标准SPI模式)
    flash = W25Q32DWDriver(spi_driver, usb_index=0, spi_mode='standard')

    # 初始化Flash
    if not flash.initialize():
        print("✗ Flash初始化失败")
        return

    # exit(0)

    # 显示设备信息
    print("\n" + "="*60)
    print("设备信息:")
    print("="*60)
    device_info = flash.get_device_info()
    for key, value in device_info.items():
        print(f"  {key:<20}: {value}")

    # Flash驱动简化测试流程
    print("\n" + "="*60)
    print("Flash驱动测试 - 写-擦-写-擦流程:")
    print("="*60)

    import random

    # 记录开始时间
    start_time = time.time()

    size = 1000

    addr1 = random.randint(0, FlashSpecs.FLASH_SIZE - 1000) & ~(FlashSpecs.SECTOR_SIZE - 1)  # 随机sector对齐地址
    data1 = [random.randint(0, 255) for _ in range(size)]  # 跨页数据
    print(f"   地址: 0x{addr1:06X}, 大小: {len(data1)}字节")

    print(f"\n 4KB扇区擦除")
    sector_addr = AddressUtils.align_to_sector(addr1)
    print(f"   扇区地址: 0x{sector_addr:06X}")

    if flash.sector_erase(sector_addr):
        print("✓ 扇区擦除完成")
        # 验证擦除效果
        success, verify_data = flash.read_data(sector_addr, 1024)
        if success and all(x == 0xFF for x in verify_data):
            print("✓ 扇区擦除验证通过")
        else:
            non_ff = sum(1 for x in verify_data if x != 0xFF) if success else -1
            print(f"✗ 扇区擦除验证失败 (有{non_ff}个非0xFF字节)")
    else:
        print("✗ 扇区擦除失败")
        return

    if flash.program_data(addr1, data1, verify=True):
        print("✓ 跨页写入成功")
    else:
        print("✗ 跨页写入失败")
        return

    # # 4. 块擦除 + 验证
    # print(f"\n🗑️  步骤4: 32KB块擦除")
    # block_addr = random.randint(0, FlashSpecs.FLASH_SIZE - 1) & ~(FlashSpecs.BLOCK_32K_SIZE - 1)  # 随机32KB块对齐地址
    # print(f"   块地址: 0x{block_addr:06X}")
    #
    # 块擦除减少寿命很严重, 少用
    # if flash.block_erase_32k(block_addr):
    #     print("✓ 32KB块擦除完成")
    #     # 验证擦除效果
    #     success, verify_data = flash.read_data(block_addr, 1024)
    #     if success and all(x == 0xFF for x in verify_data):
    #         print("✓ 块擦除验证通过")
    #     else:
    #         non_ff = sum(1 for x in verify_data if x != 0xFF) if success else -1
    #         print(f"✗ 块擦除验证失败 (有{non_ff}个非0xFF字节)")
    # else:
    #     print("✗ 32KB块擦除失败")
    #     return

    # 记录结束时间
    end_time = time.time()
    total_time = end_time - start_time

    print(f"\n⏱️ 总测试时间: {total_time:.3f}秒")

    print("\n" + "="*60)
    print("✓ Flash驱动测试完成")
    print("="*60)

    # 关闭USB设备
    spi_driver.close_usb(0)
    print("\n✓ USB设备已关闭")


if __name__ == "__main__":
    main()
