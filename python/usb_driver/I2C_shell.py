"""
Interactive I2C Register Read/Write Tool
Simple command-line interface for reading and writing MS1 registers via I2C
"""

from usb.i2c_driver import IIC_Rate_1M, IIC_Rate_100K, IIC_Rate_200K, IIC_Rate_400K, IIC_Rate_600K, IIC_Rate_800K


# I2C rate name mapping
I2C_RATE_NAMES = {
    IIC_Rate_100K: "100K",
    IIC_Rate_200K: "200K",
    IIC_Rate_400K: "400K",
    IIC_Rate_600K: "600K",
    IIC_Rate_800K: "800K",
    IIC_Rate_1M: "1M",
}


def print_help():
    """Print command help"""
    print("\n" + "=" * 70)
    print("COMMANDS")
    print("=" * 70)
    print("  r <addr>           - Read register (hex or dec)")
    print("  read <addr>")
    print()
    print("  w <addr> <value>   - Write register (hex or dec)")
    print("  write <addr> <value>")
    print()
    print("  rate <speed>       - Change I2C rate (100k/200k/400k/600k/800k/1m)")
    print()
    print("  h, help            - Show this help")
    print("  q, quit, exit      - Exit program")
    print()
    print("Examples:")
    print("  r 0xfe002040       - Read reset control register")
    print("  w 0xfe09c03c 0xf4000000  - Write boot vector")
    print("  rate 400k          - Set I2C to 400kHz")
    print("=" * 70 + "\n")


def parse_int(s):
    """Parse integer from hex or decimal string"""
    s = s.strip()
    if s.startswith('0x') or s.startswith('0X'):
        return int(s, 16)
    return int(s, 0)


def interactive_loop(ms1):
    """Main interactive command loop"""
    print("\n" + "=" * 70)
    print("MS1 I2C Register Read/Write Tool")
    print("=" * 70)
    print(f"I2C Rate: {I2C_RATE_NAMES[ms1.i2c_rate]}")
    print(f"Slave Address: 0x{ms1.slave_addr:02X}")
    print()
    print("Type 'help' for commands, 'quit' to exit")
    print("=" * 70)
    
    while True:
        try:
            # Get user input
            cmd_line = input("\nI2C> ").strip()
            
            if not cmd_line:
                continue
            
            # Parse command
            parts = cmd_line.split()
            cmd = parts[0].lower()
            
            # Help
            if cmd in ['h', 'help', '?']:
                print_help()
                continue
            
            # Quit
            if cmd in ['q', 'quit', 'exit']:
                print("\nExiting I2C shell...")
                break
            
            # Read
            if cmd in ['r', 'read']:
                assert len(parts) >= 2, "Usage: read <addr>"
                addr = parse_int(parts[1])
                
                print(f"Reading from 0x{addr:08X}...", end=' ')
                data = ms1.read(addr, 1)
                assert data is not None, "Read failed"
                
                print(f"✓")
                print(f"  Value: 0x{data[0]:08X} ({data[0]})")
                continue
            
            # Write
            if cmd in ['w', 'write']:
                assert len(parts) >= 3, "Usage: write <addr> <value>"
                addr = parse_int(parts[1])
                value = parse_int(parts[2])
                
                print(f"Writing 0x{value:08X} to 0x{addr:08X}...", end=' ')
                assert ms1.write(addr, value), "Write failed"
                
                # Read back to verify
                data = ms1.read(addr, 1)
                assert data is not None, "Read back failed"
                
                if data[0] == value:
                    print(f"✓")
                else:
                    print(f"⚠ Warning: read back 0x{data[0]:08X}, expected 0x{value:08X}")
                continue
            
            # Change I2C rate
            if cmd in ['rate', 'speed']:
                assert len(parts) >= 2, "Usage: rate <100k|200k|400k|600k|800k|1m>"
                rate_str = parts[1].lower()
                
                rate_map = {
                    '100k': IIC_Rate_100K,
                    '200k': IIC_Rate_200K,
                    '400k': IIC_Rate_400K,
                    '600k': IIC_Rate_600K,
                    '800k': IIC_Rate_800K,
                    '1m': IIC_Rate_1M,
                }
                
                assert rate_str in rate_map, f"Unknown rate: {rate_str}"
                ms1.i2c_rate = rate_map[rate_str]
                assert ms1.configure(), "Failed to reconfigure I2C"
                
                print(f"I2C rate changed to {I2C_RATE_NAMES[ms1.i2c_rate]}")
                continue
            
            # Unknown command
            print(f"Unknown command: {cmd}")
            print("Type 'help' for available commands")
            
        except KeyboardInterrupt:
            print("\n\nUse 'quit' to exit")
            continue
        except AssertionError as e:
            print(f"✗ Error: {e}")
            continue
        except Exception as e:
            print(f"✗ Error: {e}")
            continue

