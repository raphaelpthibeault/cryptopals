#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

typedef struct byte_array {
	uint8_t *bytes;
	size_t len;
} byte_array_t;

#define NO_BA (byte_array_t){NULL, -1ULL}


byte_array_t alloc_byte_array(size_t len);
void free_byte_array(byte_array_t ba);
void free_byte_arrays(byte_array_t ba, ...);

uint8_t *hex_to_base64(const char *hex_string);
uint8_t *byte_array_to_hex(const byte_array_t ba);
byte_array_t hex_to_byte_array(const char *hex_string);

uint8_t *xor_hex_strings(uint8_t *a, uint8_t *b);
void single_byte_xor(byte_array_t out, const byte_array_t a, uint8_t c);

void print_byte_array_ascii(const byte_array_t x) ;

static inline bool
char_isalpha(uint8_t c)
{
	return ('a' <= c && c <= 'z')
				|| ('A' <= c && c <= 'Z');
}

#endif // !__UTILS_H__
