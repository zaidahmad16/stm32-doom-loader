import sys
import struct

APP_SIZE = 0x10000  # 64K - must match appSize in main.c

def crc32Calculate(data):
    crc = 0xFFFFFFFF
    for byte in data:
        crc ^= byte
        for _ in range(8):
            if crc & 1:
                crc = (crc >> 1) ^ 0xEDB88320
            else:
                crc >>= 1
    return crc ^ 0xFFFFFFFF

def appendCrc(inputFile, outputFile):
    with open(inputFile, 'rb') as f:
        data = f.read()

    # pad to APP_SIZE - 4 bytes with 0xFF (blank flash value)
    padSize = (APP_SIZE - 4) - len(data)
    if padSize < 0:
        print(f"Error: binary is larger than APP_SIZE")
        sys.exit(1)

    data = data + b'\xff' * padSize
    print(f"Padded to {len(data)} bytes")

    # calculate CRC over padded binary
    crc = crc32Calculate(data)
    print(f"CRC32: 0x{crc:08X}")

    # append CRC as 4 bytes little endian
    crcBytes = struct.pack('<I', crc)

    with open(outputFile, 'wb') as f:
        f.write(data)
        f.write(crcBytes)

    print(f"Written to {outputFile} ({len(data) + 4} bytes)")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 append_crc.py <input.bin> <output.bin>")
        sys.exit(1)

    appendCrc(sys.argv[1], sys.argv[2])