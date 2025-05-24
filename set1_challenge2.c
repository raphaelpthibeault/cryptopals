#include <stdio.h>
#include <lib/utils.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	uint8_t *res = xor_hex_strings(argv[1], argv[2]);
	printf("%s\n", res);
	free(res);
	return 0;
}
