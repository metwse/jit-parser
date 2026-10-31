#define _GNU_SOURCE

#include "osal.h"

#include <sys/mman.h>
#include <unistd.h>


#define round(size) \
	((size + _SC_PAGE_SIZE - 1) / _SC_PAGE_SIZE) * _SC_PAGE_SIZE


int execmem_alloc(struct execmem *out, size_t size)
{
	void *mem = mmap(NULL,
			 round(size),
			 PROT_READ | PROT_WRITE,
			 MAP_PRIVATE | MAP_ANON, -1, 0);

	if (mem == (void *) -1)
		return 1;

	*out = (struct execmem) {
		.mode = EXECMEM_RW,
		.mem = mem,
		.size = size,
	};

	return 0;
}

void execmem_free(struct execmem *e)
{
	munmap(e, e->size);
}

int execmem_realloc(struct execmem *e, size_t size)
{
	if (size < e->size)
		return 0;

	void *mem = mremap(e->mem, e->size, round(size), MREMAP_MAYMOVE);

	if (mem == (void *) -1)
		return 1;

	e->mem = mem;
	e->size = round(size);

	return 0;
}

void execmem_set_mode(struct execmem *e, enum execmem_mode mode)
{
	assert2(mmap(e->mem, e->size,
		mode == EXECMEM_RW ? PROT_READ | PROT_WRITE : PROT_EXEC,
		MAP_ANON, -1, 0) != (void *) -1,
		"could change memory mode");
}
