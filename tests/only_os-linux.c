#include "../osal/osal.h"


int main(void)
{
	struct execmem e1, e2;

	execmem_xalloc(&e1, 12);
	execmem_xalloc(&e2, 5432);

	assert2(e1.size >= 12,);
	assert2(e2.size >= 5432,);

	for (size_t i = 0; i < 5432 / sizeof(size_t); i++)
		((size_t *) e2.mem)[i] = i;

	execmem_xrealloc(&e2, 8765);
	execmem_xrealloc(&e1, 4321);

	for (size_t i = 0; i < 5432 / sizeof(size_t); i++)
		assert2(((size_t *) e2.mem)[i] == i,);

	assert2(e1.size >= 4312,);
	execmem_free(&e1);

	assert2(e2.size >= 8765,);
	execmem_free(&e2);
}
