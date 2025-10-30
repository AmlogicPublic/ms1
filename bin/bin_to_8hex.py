import sys
import struct

def bin2mem(input_file, output_file):
    with open(input_file, 'rb') as f:
        binary_data = f.read()
    
    with open(output_file, 'w') as f:
        # Read 4 bytes at a time (32-bit words)
        for i in range(0, len(binary_data), 4):
            chunk = binary_data[i:i+4]
            
            # Pad with zeros if less than 4 bytes
            if len(chunk) < 4:
                chunk = chunk + b'\x00' * (4 - len(chunk))
            
            # Unpack as little-endian 32-bit unsigned integer
            word = struct.unpack('<I', chunk)[0]
            
            # Write as 8-digit hexadecimal (without 0x prefix)
            f.write(f'{word:08x}\n')

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print(f'Usage: {sys.argv[0]} <input.bin> <output.mem>')
        sys.exit(1)
    
    bin2mem(sys.argv[1], sys.argv[2])

