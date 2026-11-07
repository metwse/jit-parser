#include "../osal/osal.h"
#include "../src/common.h"


int main(void)
{
	struct mas_execmem e1, e2;

	mas_execmem_xalloc(&e1, 12);
	mas_execmem_xalloc(&e2, 5432);

	assert2(e1.size >= 12,);
	assert2(e2.size >= 5432,);

	for (size_t i = 0; i < 5432 / sizeof(size_t); i++)
		((size_t *) e2.mem)[i] = i;

	mas_execmem_free(&e1);
	mas_execmem_free(&e2);
}
