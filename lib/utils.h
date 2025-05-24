#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include <stdlib.h>

typedef struct byte_array {
	uint8_t *bytes;
	size_t len;
} byte_array_t;

#define NO_BA (byte_array_t){NULL, -1ULL}

uint8_t *hex_to_base64(const char *hex_string);

uint8_t *xor_hex_strings(uint8_t *a, uint8_t *b);

#endif // !__UTILS_H__
