from pyftdi.i2c import I2cController, I2cNackError
from pyftdi.ftdi import Ftdi
import usb.core
import sys
import time
import random
from tqdm import tqdm

# ============================================================
# CONFIGURATION - Adjust these parameters as needed
# ============================================================

# Device configuration
DEVICE_URL = 'ftdi://ftdi:2232h/1'  # FTDI device URL (Interface 1)
SLAVE_ADDR = 0x3d                    # I2C slave address

# Memory configuration
MEM_BASE = 0xF4000000                # Memory base address
MEM_SIZE = 1 * 1024 * 1024           # Total memory size: 1MB

# Speed scan configuration
SPEED_SCAN_FREQUENCIES = [           # Test frequencies (Hz)
    10000,  
    50000,  
    100000, 
    200000, 
    400000, 
    800000, 
    1000000,
    2000000,
    3000000 
]

SPEED_TEST_SIZE = 1 * 1024           # Speed test size: 1KB (fast)
SPEED_VERIFY_INTERVAL = 5           # Verify every N writes (4B per write)

# Display configuration
SHOW_DEVICE_SCAN = True              # Show detailed device scan info

# ============================================================

def get_driver_type(device):
    """Detect USB driver type"""
    usb_dev = usb.core.find(idVendor=device.vid, idProduct=device.pid, bus=device.bus, address=device.address)
    if not usb_dev:
        return "Unknown"
    backend_name = type(usb_dev._ctx.backend).__module__  # type: ignore
    return "WinUSB/libusb-1.0" if 'libusb1' in backend_name else "libusb-win32" if 'libusb0' in backend_name else backend_name

def print_progress(current, total, start_time, prefix="", show_bar=True):
    """Print compact progress with optional bar"""
    percent = current / total * 100
    elapsed = time.time() - start_time
    speed = (current / 1024) / elapsed if elapsed > 0 else 0
    
    if show_bar:
        bar_length = 15
        filled = int(bar_length * current / total)
        bar = '█' * filled + '░' * (bar_length - filled)
        sys.stdout.write(f"\r{prefix}[{bar}] {current/1024:.1f}KB {speed:5.1f}KB/s")
    else:
        sys.stdout.write(f"\r{prefix}{current/1024:.1f}KB {speed:5.1f}KB/s")
    sys.stdout.flush()

def mem_write_4b(slave, addr, data32):
    """Write 4 bytes to memory address (addr is 32-bit, data is 32-bit)"""
    # Write: [addr[3], addr[2], addr[1], addr[0], data[3], data[2], data[1], data[0]]
    addr_bytes = [(addr >> (8*i)) & 0xFF for i in range(4)]
    data_bytes = [(data32 >> (8*i)) & 0xFF for i in range(4)]
    slave.write(addr_bytes + data_bytes)

def mem_read_4b(slave, addr):
    """Read 4 bytes from memory address"""
    addr_bytes = [(addr >> (8*i)) & 0xFF for i in range(4)]
    data = slave.exchange(addr_bytes, 4)
    return int.from_bytes(data, byteorder='little')

def memory_test(slave, frequency, test_size=100*1024, verify_interval=256):
    """
    Memory test: Write + Random Read + Full Read
    """
    num_regs = test_size // 4
    test_data = {}
    random_verify_count = 0
    
    # Phase 1: Write with random verify
    write_start = time.time()
    with tqdm(total=num_regs, desc="  Write", unit="reg", unit_scale=False, 
              bar_format='{desc}: {n_fmt}/{total_fmt} regs [{bar:20}] {rate_fmt}', 
              leave=False, ncols=80) as pbar:
        for i in range(num_regs):
            addr = MEM_BASE + i * 4
            data = random.randint(0, 0xFFFFFFFF)
            test_data[addr] = data
            mem_write_4b(slave, addr, data)
            
            if (i + 1) % verify_interval == 0 and i > 0:
                verify_addr = MEM_BASE + random.randint(0, i) * 4
                read_data = mem_read_4b(slave, verify_addr)
                assert read_data == test_data[verify_addr], f"Random verify failed"
                random_verify_count += 1
            
            pbar.update(1)
    
    write_time = time.time() - write_start
    write_speed = (test_size / 1024) / write_time
    
    # Phase 2: Full Read
    verify_start = time.time()
    errors = 0
    
    with tqdm(total=num_regs, desc="  Verify", unit="reg", unit_scale=False,
              bar_format='{desc}: {n_fmt}/{total_fmt} regs [{bar:20}] {rate_fmt}',
              leave=False, ncols=80) as pbar:
        for i, (addr, expected) in enumerate(test_data.items()):
            read_data = mem_read_4b(slave, addr)
            if read_data != expected:
                errors += 1
            pbar.update(1)
    
    verify_time = time.time() - verify_start
    verify_speed = (test_size / 1024) / verify_time
    status = "✓" if errors == 0 else f"✗{errors}err"
    
    # Final result - compact single line
    print(f"  {frequency/1000:>6.0f}kHz: W:{test_size/1024:4.1f}KB {write_speed:5.1f}KB/s {write_time:5.2f}s | RandVerify:{random_verify_count:3}x | FullRead:{test_size/1024:4.1f}KB {verify_speed:5.1f}KB/s {verify_time:5.2f}s {status}")
    
    return errors == 0

# Scan and display FTDI devices
if SHOW_DEVICE_SCAN:
    print("Scanning for FTDI devices...")
    print("=" * 60)
    Ftdi.show_devices()
    print("=" * 60)

    devices = Ftdi.list_devices()
    assert devices, "ERROR: No FTDI devices found!"

    print(f"\nFound {len(devices)} FTDI interface(s):")
    for idx, (device, interface) in enumerate(devices):
        print(f"\n  Interface {idx+1}:")
        print(f"    VID:PID = {device.vid:04x}:{device.pid:04x}")
        print(f"    Bus:Address = {device.bus}:{device.address}")
        print(f"    Description: {device.description}")
        print(f"    Serial: {device.sn or 'None'}")
        print(f"    Driver: {get_driver_type(device)}")
        print(f"    URL: ftdi://ftdi:2232:{device.bus}:{device.address}/{interface}")
    print()

print(f"Using device URL: {DEVICE_URL}")

# I2C speed scan with memory test
print(f"\nI2C SPEED SCAN - {SPEED_TEST_SIZE/1024:.0f}KB test, verify every {SPEED_VERIFY_INTERVAL} writes")
print("=" * 100)
print(f"  {'Freq':>6} | {'Write Phase':^30} | {'Random Verify':^12} | {'Full Read Phase':^30} | Status")
print(f"  {'-'*6}-+-{'-'*30}-+-{'-'*12}-+-{'-'*30}-+-{'-'*6}")

for freq in SPEED_SCAN_FREQUENCIES:
    try:
        i2c = I2cController()
        i2c.configure(url=DEVICE_URL, frequency=freq)  # type: ignore
        slave = i2c.get_port(SLAVE_ADDR)
        slave.configure_register(width=4)
        
        memory_test(slave, freq, test_size=SPEED_TEST_SIZE, verify_interval=SPEED_VERIFY_INTERVAL)
        
    except I2cNackError:
        print(f"  {freq/1000:>6.0f}kHz: ✗ NACK Error")
    except Exception as e:
        print(f"  {freq/1000:>6.0f}kHz: ✗ {type(e).__name__}: {str(e)[:50]}")

print("=" * 100)
