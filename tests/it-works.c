#include "../include/add.h"

#include <assert.h>


int main(void)
{
	assert(add(2, 3) == 5);
	assert(add_arm(1, 4) == 5);
}
