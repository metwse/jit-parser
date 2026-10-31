#include "../../include/add.h"


int add_arm(int a, int b)
{
	int c = 0;

	__asm__ volatile (
		".syntax unified\n"
		".thumb\n"
		"adds %0, %1, %2"
		: "+r" (c)
		: "r" (a), "r" (b)
	);

	return c;
}
