#ifndef CRC32_HPP
#define CRC32_HPP

#ifdef _WIN32
#pragma once
#endif

#include <inttypes.h>

/* CRC-32C (iSCSI) polynomial in reversed bit order. */
#define POLY 0x82f63b78

/* CRC-32 (Ethernet, ZIP, etc.) polynomial in reversed bit order. */
/* #define POLY 0xedb88320 */

uint32_t CRC32(const unsigned char *szBuffer, size_t nLength);

#endif