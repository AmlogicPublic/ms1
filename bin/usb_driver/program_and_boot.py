"""
Unified MS1 Programming and Boot Tool
Configurable flow for SRAM/Flash programming with UART monitoring and I2C shell
"""

import os
import sys

# Add script directory to path so we can import local modules
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, SCRIPT_DIR)

import time
import random
import threading
from collections import deque
from datetime import datetime
from usb.usb_driver_top import USBDriverTop
from ms1.ms1_i2c import MS1I2CDriver
from usb.i2c_driver import IIC_Rate_1M, IIC_Rate_100K, IIC_Rate_200K, IIC_Rate_400K, IIC_Rate_600K, IIC_Rate_800K
from usb.uart_driver import UART_Parity_No, UART_StopBits_1
from I2C_shell import interactive_loop

# ============================================================================
# CONFIGURATION - Modify these settings for different use cases
# ============================================================================

# Boot location
BOOT_FROM = "sram"  # Options: "sram", "flash"

# On-end behavior
END_MODE = "i2c_shell"  # Options: "uart_shell", "i2c_shell"

# I2C settings
I2C_RATE = IIC_Rate_1M  # Options: IIC_Rate_100K, 200K, 400K, 600K, 800K, 1M

# UART settings
UART_BAUD = 115200
UART_PARITY = UART_Parity_No
UART_STOP_BITS = UART_StopBits_1
UART_SHOW_HEX = False  # True for hex display, False for ASCII

# ============================================================================
# Internal Constants
# ============================================================================

MEM_FILE = os.path.join(SCRIPT_DIR, "../../build/ms1.mem")
SRAM_BASE = 0xF4000000
FLASH_BASE = 0xF0000000
SAMPLE_INTERVAL = 10

# Register addresses
RESETCTRL_RESET0_LEVEL = 0xfe002040
AOCPU_CPU_VECTOR = 0xfe09c03c
AOCPU_CPU_CTRL1 = 0xfe09c004
AOCPU_CPU_STS0 = 0xfe09c028
AOCPU_CPU_STS1 = 0xfe09c02c

# I2C rate name mapping
I2C_RATE_NAMES = {
    IIC_Rate_100K: "100K",
    IIC_Rate_200K: "200K",
    IIC_Rate_400K: "400K",
    IIC_Rate_600K: "600K",
    IIC_Rate_800K: "800K",
    IIC_Rate_1M: "1M",
}


# ============================================================================
# Utility Functions
# ============================================================================

def load_mem_file(filepath):
    """Load .mem file and parse hex values"""
    data = []
    with open(filepath, 'r') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            value = int(line, 16)
            data.append(value)
    return data


def write_register_verify(ms1, addr, value, reg_name=""):
    """Write register and verify it was written correctly"""
    name = reg_name if reg_name else f"0x{addr:08X}"
    
    before = ms1.read(addr, 1)
    if before is not None:
        print(f"  {name} before: 0x{before[0]:08X}")
    
    print(f"  Writing: 0x{value:08X}")
    assert ms1.write(addr, value), f"Failed to write {name}"
    
    read_back = ms1.read(addr, 1)
    assert read_back is not None, f"Failed to read back {name}"
    assert read_back[0] == value, f"{name} verify failed: wrote 0x{value:08X}, read 0x{read_back[0]:08X}"
    
    print(f"  {name} after: 0x{read_back[0]:08X} ✓")


def hold_cpu(ms1):
    """Hold CPU in reset"""
    read_data = ms1.read(RESETCTRL_RESET0_LEVEL, 1)
    assert read_data is not None
    new_value = read_data[0] & ~0x6
    write_register_verify(ms1, RESETCTRL_RESET0_LEVEL, new_value, "RESET0_LEVEL")


def release_cpu(ms1):
    """Release CPU from reset"""
    read_data = ms1.read(RESETCTRL_RESET0_LEVEL, 1)
    assert read_data is not None
    new_value = read_data[0] | 0x6
    write_register_verify(ms1, RESETCTRL_RESET0_LEVEL, new_value, "RESET0_LEVEL")


# ============================================================================
# UART Receiver
# ============================================================================

class UARTReceiver:
    """Background UART receiver with optional file logging"""
    def __init__(self, uart, usb_index=0, log_file=None):
        self.uart = uart
        self.usb_index = usb_index
        self.buffer = deque()
        self.running = False
        self.thread = None
        self.lock = threading.Lock()
        self.log_file = log_file
        self.log_handle = None
        
        if self.log_file:
            self.log_handle = open(self.log_file, 'wb')
    
    def _receive_loop(self):
        """Background thread to continuously receive data"""
        while self.running:
            success, data = self.uart.receive_data(4096, self.usb_index)
            if success and data:
                with self.lock:
                    self.buffer.extend(data)
                    if self.log_handle:
                        self.log_handle.write(bytes(data))
                        self.log_handle.flush()
            time.sleep(0.001)
    
    def start(self):
        """Start background receiving"""
        self.running = True
        self.thread = threading.Thread(target=self._receive_loop, daemon=True)
        self.thread.start()
    
    def stop(self):
        """Stop background receiving"""
        self.running = False
        if self.thread:
            self.thread.join(timeout=1.0)
        if self.log_handle:
            self.log_handle.close()
            self.log_handle = None
    
    def get_buffered_data(self):
        """Get all buffered data"""
        with self.lock:
            data = list(self.buffer)
            self.buffer.clear()
            return data


# ============================================================================
# End Mode Handlers
# ============================================================================

def monitor_uart_shell(receiver):
    """Display UART data in terminal (blocking)"""
    mode = "HEX" if UART_SHOW_HEX else "ASCII"
    print("\n" + "=" * 70)
    print(f"UART Monitor: {UART_BAUD} baud, {mode} mode")
    print("=" * 70)
    print("Press Ctrl+C to stop")
    print("=" * 70 + "\n")
    
    try:
        while True:
            data = receiver.get_buffered_data()
            if data:
                if UART_SHOW_HEX:
                    hex_str = ' '.join(f'{b:02X}' for b in data)
                    print(hex_str, flush=True)
                else:
                    text = bytes(data).decode('utf-8', errors='replace')
                    print(text, end='', flush=True)
            time.sleep(0.01)
    except KeyboardInterrupt:
        print("\n" + "=" * 70)
        print("UART monitoring stopped")
        print("=" * 70)


def start_i2c_shell(ms1):
    """Enter I2C interactive shell (blocking)"""
    interactive_loop(ms1)


# ============================================================================
# CPU Status Report
# ============================================================================

def cpu_status_report(ms1):
    """Read and report AOCPU status"""
    print("\n" + "=" * 70)
    print("CPU STATUS")
    print("=" * 70)
    
    boot_vec = ms1.read(AOCPU_CPU_VECTOR, 1)
    ctrl1 = ms1.read(AOCPU_CPU_CTRL1, 1)
    sts1 = ms1.read(AOCPU_CPU_STS1, 1)
    assert boot_vec and ctrl1 and sts1
    
    print(f"Boot:0x{boot_vec[0]:08X}  CTRL1:0x{ctrl1[0]:08X}  STS1:0x{sts1[0]:08X}")
    print()
    
    c = ctrl1[0]
    print(f"CTRL1: AddrShift={c>>31&1} Sleep={c>>10&1} RXEV={c>>9&1} DebugSel={c>>6&3} Stop={c>>1&1} PC={c&1}")
    print()
    
    s = sts1[0]
    priv = {0:"U", 1:"S", 3:"M"}.get(s>>11&3, "?")
    
    print(f"STS1:  TxEvtCnt={s>>14&0x3F:2d}  Priv={priv}")
    print(f"       Sleep={s>>13&1}  Invalid={s>>9&1}  Exception={s>>8&1}  Interrupt={s>>7&1}")
    print(f"       Halted={s>>6&1}  NDMReset={s>>5&1}  DMActive={s>>4&1}")
    print(f"       StopTime={s>>3&1}  HS={s>>2&1}  HD={s>>1&1}  SysReq={s&1}")
    print()
    
    print("STS0 Trace:")
    ctrl1_backup = c
    for sel in range(4):
        ms1.write(AOCPU_CPU_CTRL1, (ctrl1_backup & ~(0x3<<6)) | (sel<<6))
        sts0 = ms1.read(AOCPU_CPU_STS0, 1)
        if sts0:
            val = sts0[0]
            name = ["cause","tval","iaddr","instr"][sel]
            extra = ""
            if sel == 0 and val:
                causes = {2:"IllInst", 3:"Break", 11:"Ecall-M"}
                extra = f" ({causes.get(val&0x1F, f'code:{val&0x1F}')})"
            elif sel == 3 and val:
                extra = f" (op:0x{val&0x7F:02X})"
            print(f"  {name:6s}= 0x{val:08X}{extra}")
    
    ms1.write(AOCPU_CPU_CTRL1, ctrl1_backup)
    print("=" * 70)


# ============================================================================
# Boot Implementations
# ============================================================================

def boot_from_sram(ms1, code_data):
    """Program and boot from internal SRAM"""
    total_words = len(code_data)
    total_bytes = total_words * 4
    
    print(f"\n[Step 2] Hold CPU")
    hold_cpu(ms1)
    
    print(f"\n[Step 3] Write SRAM")
    print(f"  Target: 0x{SRAM_BASE:08X}")
    print(f"  Size: {total_words} words ({total_bytes / 1024:.1f} KB)")
    print(f"  Writing with random sampling...")
    
    for i in range(total_words):
        addr = SRAM_BASE + i * 4
        assert ms1.write(addr, code_data[i]), f"Write failed at 0x{addr:08X}"
        
        if i > 0 and (i % SAMPLE_INTERVAL == 0 or i == total_words - 1):
            for sample_idx in random.sample(range(i), min(3, i)):
                sample_addr = SRAM_BASE + sample_idx * 4
                read_back = ms1.read(sample_addr, 1)
                assert read_back is not None and read_back[0] == code_data[sample_idx], \
                    f"Data corrupted at 0x{sample_addr:08X}"
        
        percent = ((i + 1) * 100) // total_words
        bar = '█' * (50 * (i + 1) // total_words) + '░' * (50 - 50 * (i + 1) // total_words)
        print(f"\r  [{bar}] {percent:3d}% | {i+1}/{total_words}", end='')
    print(" ✓")
    
    print(f"  Verifying {total_words} words...", end='')
    for i in range(total_words):
        read_data = ms1.read(SRAM_BASE + i * 4, 1)
        assert read_data is not None and read_data[0] == code_data[i], \
            f"Verify failed at 0x{SRAM_BASE + i * 4:08X}"
    print(" ✓")
    
    print(f"\n[Step 4] Set Boot Vector")
    write_register_verify(ms1, AOCPU_CPU_VECTOR, SRAM_BASE, "CPU_VECTOR")
    
    print(f"\n[Step 5] Release CPU")
    release_cpu(ms1)
    
    return SRAM_BASE


def boot_from_flash(ms1, code_data):
    """Program flash and boot from it"""
    print(f"\n[Flash Boot] Not yet implemented")
    print(f"  Placeholder for flash programming flow")
    # TODO: Implement flash programming
    return FLASH_BASE


# ============================================================================
# Main Flow
# ============================================================================

def main():
    """Main unified flow"""
    print("=" * 70)
    print("MS1 Unified Programming and Boot Tool")
    print("=" * 70)
    print(f"Boot from: {BOOT_FROM.upper()}")
    print(f"End mode: {END_MODE}")
    print(f"I2C rate: {I2C_RATE_NAMES[I2C_RATE]}")
    print("=" * 70)
    
    # Validate configuration
    assert BOOT_FROM in ["sram", "flash"], "BOOT_FROM must be 'sram' or 'flash'"
    assert END_MODE in ["uart_shell", "i2c_shell"], "END_MODE must be 'uart_shell' or 'i2c_shell'"
    
    # Load code
    code_data = load_mem_file(MEM_FILE)
    total_words = len(code_data)
    total_bytes = total_words * 4
    print(f"Loaded: {total_words} words ({total_bytes / 1024:.1f} KB)")
    
    # Create USB driver top
    usb_top = USBDriverTop()
    assert usb_top.open_usb(0), "Failed to open USB device"
    
    # Create MS1 I2C driver
    ms1 = MS1I2CDriver(usb_top, usb_index=0)
    ms1.i2c_rate = I2C_RATE
    assert ms1.configure(), "Failed to configure I2C"
    assert ms1.check_slave(), "Slave not detected"
    print(f"I2C: {I2C_RATE_NAMES[ms1.i2c_rate]}, Slave 0x{ms1.slave_addr:02X}\n")
    
    # Configure UART
    print("[Step 1] Configure UART")
    uart = usb_top.uart
    assert uart is not None, "UART driver not initialized"
    assert uart.config_uart_param(UART_BAUD, UART_PARITY, UART_STOP_BITS, 0), "Failed to configure UART"
    
    # Setup UART receiver based on end mode
    if END_MODE == "i2c_shell":
        # Log to file when in I2C shell mode
        uart_log_file = os.path.join(SCRIPT_DIR, "uart_log.txt")
        print(f"  UART log file: {uart_log_file}")
        uart_receiver = UARTReceiver(uart, 0, log_file=uart_log_file)
    else:
        # No file logging in terminal mode
        uart_receiver = UARTReceiver(uart, 0, log_file=None)
    
    uart_receiver.start()
    print(f"  UART receiver started ({UART_BAUD} baud)")
    
    try:
        # Boot according to configuration
        if BOOT_FROM == "sram":
            boot_addr = boot_from_sram(ms1, code_data)
        else:  # flash
            boot_addr = boot_from_flash(ms1, code_data)
        
        # CPU Status
        print("\n[Step 6] CPU Status")
        cpu_status_report(ms1)
        
        print("=" * 70)
        print(f"✓ CPU running from 0x{boot_addr:08X}")
        print("=" * 70)
        
        # Enter end mode
        if END_MODE == "uart_shell":
            print(f"\n[Step 7] UART Terminal Monitor")
            monitor_uart_shell(uart_receiver)
        else:  # i2c_shell
            print(f"\n[Step 7] I2C Interactive Shell")
            print(f"  UART logging to: {uart_log_file}")
            start_i2c_shell(ms1)
    
    finally:
        # Cleanup
        uart_receiver.stop()
        usb_top.close_usb(0)
        print("\nCleaned up resources")
    
    return True


if __name__ == "__main__":
    success = main()
    exit(0 if success else 1)

