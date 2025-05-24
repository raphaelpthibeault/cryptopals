#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include <stdlib.h>

struct byte_array {
	uint8_t *bytes;
	size_t len;
};

uint8_t *hex_to_base64(const char *hex_string);

#endif // !__UTILS_H__
