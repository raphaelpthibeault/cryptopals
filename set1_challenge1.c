#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <lib/utils.h>

int
main(int argc, char *argv[])
{
	/* set 1 challenge 1 */
	uint8_t *base64_str = hex_to_base64(argv[1]);

	printf("%s\n", base64_str);
	free(base64_str);
	return 0;
}
