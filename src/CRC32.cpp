#include "CRC32.hpp"

uint32_t CRC32(const unsigned char *szBuffer, size_t nLength) {
	int k;
	uint32_t nCRC = 0;

	nCRC = ~nCRC;
	while (nLength--) {
		nCRC ^= *szBuffer++;
		for (k = 0; k < 8; k++)
			nCRC = (nCRC >> 1) ^ (POLY & (0 - (nCRC & 1)));
	}
	return ~nCRC;
}