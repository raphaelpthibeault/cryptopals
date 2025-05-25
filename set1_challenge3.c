#include <stdio.h>
#include <lib/utils.h>
#include <cryptography/cryptopals.h>

int
main(int argc, char *argv[])
{
	/* Given hex-encoded string, find out if it has been xor'd agaist a single character */
	// Devise some method for "scoring" a piece of English plaintext. Character frequency is a good metric. Evaluate each output and choose the one with the best score.

	byte_array_t cipher = hex_to_byte_array(argv[1]);	
	score_single_byte_xor(cipher);
	free_byte_array(cipher);

	return 0;
}

