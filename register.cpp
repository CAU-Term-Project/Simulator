#include <stdio.h>
#include "defines.h"

unsigned int REG(unsigned int A, unsigned int V, unsigned int nRW) {
	if (A < 0 || A > REG_SIZE) {
		printf("Register Index Error\n");
		return -1;
	}
	if (nRW > 1) {
		printf("Invalid Control Number\n");
		return -1;
	}

	if (nRW == 0) { // read
		return R[A];
	}
	else if (nRW == 1) { // write
		R[A] = V;
	}
	return 0;
}