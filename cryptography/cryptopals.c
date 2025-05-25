#include "cryptopals.h"
#include <lib/utils.h>
#include <stdio.h>
#include <stdint.h>

void
score_single_byte_xor(const byte_array_t cipher)
{
	uint8_t key_byte = 0;
	int score;

	/* check for char frequency in the string */

	byte_array_t res = alloc_byte_array(cipher.len); 
	do {
		score = 0;
		single_byte_xor(res, cipher, key_byte);
		for (size_t i = 0; i < res.len; ++i) {
			if (char_isalpha(res.bytes[i]) || res.bytes[i] == ' ') {
				++score;
			} 	
		}

		if (score > res.len * 0.87) {
			printf("key = 0x%02hhx\tscore = %i\tlen = %li\n", key_byte, score, res.len);
			print_byte_array_ascii(res); 
		}
		
		++key_byte;
	} while (key_byte != 0); // stop upon overflow

	free_byte_array(res);
}


