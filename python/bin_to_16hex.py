import sys
import struct

def bin2mem(input_file, output_file):
    with open(input_file, 'rb') as f:
        binary_data = f.read()
    
    with open(output_file, 'w') as f:
        # Read 8 bytes at a time (two 32-bit words)
        for i in range(0, len(binary_data), 8):
            chunk = binary_data[i:i+8]
            # Pad with zeros if less than 8 bytes
            if len(chunk) < 8:
                chunk = chunk + b'\x00' * (8 - len(chunk))
            # Low word (first 4 bytes, little-endian)
            word1 = struct.unpack('<I', chunk[0:4])[0]
            # High word (next 4 bytes, little-endian)
            word2 = struct.unpack('<I', chunk[4:8])[0]
            # Concatenate the two words, each as 8 hex digits (high word on left, low word on right)
            f.write(f'{word2:08x}{word1:08x}\n')

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print(f'Usage: {sys.argv[0]} <input.bin> <output.mem>')
        sys.exit(1)
    
    bin2mem(sys.argv[1], sys.argv[2])

