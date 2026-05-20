#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>

/* Calculate CRC32 over a block of data, data = pointer to start of data, length = number of bytes to check */

uint32_t crc32Calculate(const uint8_t *data, uint32_t length);

#endif
