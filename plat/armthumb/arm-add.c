#include "../../include/add.h"


int add_arm(int a, int b)
{
	int c;

	__asm__ volatile (
		"add %0, %1, %2"
		: "+r" (c)
		: "r" (a), "r" (b)
	);

	return c;
}
