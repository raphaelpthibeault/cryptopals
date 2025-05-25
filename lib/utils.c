#include "utils.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

byte_array_t 
alloc_byte_array(size_t len) 
{
	byte_array_t ba;
	ba.len = len;
	ba.bytes = malloc(len * sizeof(uint8_t));
	memset(ba.bytes, 0, len);
	return ba;	
}

void 
free_byte_array(byte_array_t ba)
{
	if (ba.bytes != NULL) {
		free(ba.bytes);
	}
}

void
free_byte_arrays(byte_array_t ba, ...)
{
	free_byte_array(ba);

	va_list ap;
	va_start(ap, ba);
	while (1) {
		byte_array_t x = va_arg(ap, byte_array_t);
		if (x.bytes == NULL) {
			return;
		}
		free_byte_array(x);
	}
}

uint8_t
hex_char_to_byte(uint8_t c) {
	if ('0' <= c && c <= '9') {
		return c - '0';	
	} else if ('a' <= c && c <= 'f') {
		return c - 'a' + 10;
	} else if ('A' <= c && c <= 'F') {
		return c - 'A' + 10;
	}
}

byte_array_t
hex_to_byte_array(const char *hex_string)
{
	/*
		* we're talking converting to an array of bytes,
		*		we're not evaluating the hex number
		* byte_array being an array of 8-bit entries uint8_t obviously 
		* in 8 bits we can fit 2 hex, [hex1,hex2]
		* hex1 - high nibble,  << 2
		* hex2 - low nibble, << 0
		* slight complication with even/odd input length
		* */

	size_t len = strlen(hex_string);
	byte_array_t ba = alloc_byte_array((len+1)/2);
	uint8_t byte;

	size_t off = len&1; /* 1 if odd, 0 if even */
	for (size_t i = 0; i < len; ++i) {
		byte = hex_char_to_byte(hex_string[i]);
		ba.bytes[(i+off)/2] += byte << ((1^off^(i&1))<<2);
	}	

	return ba;
}

uint8_t hex_lookup[16] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
};

uint8_t *
byte_array_to_hex(const byte_array_t ba)
{
	/* in 8 bits we have a high nibble and a low nibble each being a hex
	 * [hex1][hex2]
	 * meaning 4-bit values
	 * */	

	size_t num_4_bit_strings = (ba.len << 1) + 1;
	uint8_t *ret = malloc(num_4_bit_strings * sizeof(uint8_t));
	memset(ret, 0, num_4_bit_strings);

	uint8_t *p = ret;

	for (size_t i = 0; i < ba.len; ++i, p+=2) {
		// want to get the same byte twice
		// if i is even: left nibble, else: right nibble

		p[0] = hex_lookup[(ba.bytes[i]>>4)&0xf];
		p[1] = hex_lookup[ba.bytes[i]&0xf];
	}
	
	ret[num_4_bit_strings-1] = '\0';
	return ret;
}

uint8_t base64_lookup[64] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
};

uint8_t *
three_bytes_to_base64(uint8_t *p, uint8_t b0, uint8_t b1, uint8_t b2)
{
	*p++ = base64_lookup[b0 >> 2];
	*p++ = base64_lookup[((b0 & 0x3) << 4) + (b1 >> 4)];
	*p++ = base64_lookup[((b1 & 0xf) << 2) + (b2 >> 6)];
	*p++ = base64_lookup[b2 & 0x3f];
	return p;
}

uint8_t *
byte_array_to_base64(byte_array_t ba)
{
	/* 3 byte (8-bit) values are joined together in a string 
	 * groups of 6 bits (6 bits have a maximum of 26 = 64 different binary values) are converted into individual numbers from start to end 
	 * there are 4 characters in a 24-bit string, which are then converted into their corresponding Base64 character values. 
	 * */	
	
	size_t num_24_bit_strings = ba.len / 3;
	size_t leftover_bits = (ba.len % 3);
	size_t base64_string_size = (((ba.len + 2) / 3) << 2) + 1; // 4 characters for each 3 byte chunk and null byte

	uint8_t *base64_str = malloc(base64_string_size * sizeof(uint8_t));
	memset(base64_str, 0, base64_string_size);

	uint8_t *p = base64_str;
	
	for (size_t i = 0; i < num_24_bit_strings; ++i) {
		p = three_bytes_to_base64(p, ba.bytes[3*i], ba.bytes[3*i+1], ba.bytes[3*i+2]);
	}

	if (leftover_bits == 2) {
		p = three_bytes_to_base64(p, ba.bytes[3*num_24_bit_strings], ba.bytes[3*num_24_bit_strings+1], 0);
		p[-1] = '=';
	} else if (leftover_bits == 1) {
		p = three_bytes_to_base64(p, ba.bytes[3*num_24_bit_strings], 0, 0);
		p[-1] = '=';
		p[-2] = '=';
	}

	return base64_str;
}

uint8_t *
hex_to_base64(const char *hex_string)
{
	/* always operate on raw bytes, never encoded strings ; only use hex and base64 for pretty-printing */
	byte_array_t ba = hex_to_byte_array(hex_string);
	uint8_t *base64_str = byte_array_to_base64(ba);
	free_byte_array(ba);
	return base64_str;
}

byte_array_t
xor_byte_arrays(const byte_array_t a, const byte_array_t b)
{
	size_t count = (a.len > b.len) ? b.len : a.len;
	byte_array_t ret = alloc_byte_array(count);

	for (size_t i = 0; i < count; ++i) {
		ret.bytes[i] = a.bytes[i] ^ b.bytes[i];
	}

	return ret;
}

void
single_byte_xor(byte_array_t out, const byte_array_t a, uint8_t c)
{
	for (size_t i = 0; i < a.len; ++i) {
		out.bytes[i] = a.bytes[i] ^ c;
	}
}

uint8_t *
xor_hex_strings(uint8_t *a, uint8_t *b)
{
	byte_array_t x = hex_to_byte_array(a);
	byte_array_t y = hex_to_byte_array(b);
	byte_array_t z = xor_byte_arrays(x, y);
	uint8_t *ret = byte_array_to_hex(z);

	// ret is alloc'd, so we are safe to free z
	free_byte_arrays(x, y, z, NO_BA);
	return ret;
}


void 
print_byte_array_ascii(const byte_array_t x) 
{
	for (size_t i = 0; i < x.len ; i++) {
		printf("%c", x.bytes[i]);
	}
	printf("\n");
}

