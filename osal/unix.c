#include "osal.h"
#include "../src/common.h"

#include <sys/mman.h>
#include <unistd.h>


#define round(size) \
	((size + _SC_PAGE_SIZE - 1) / _SC_PAGE_SIZE) * _SC_PAGE_SIZE


int mas_execmem_alloc(struct mas_execmem *out, size_t size)
{
	void *mem = mmap(NULL,
			 round(size),
			 PROT_READ | PROT_WRITE,
			 MAP_PRIVATE | MAP_ANON, -1, 0);

	if (mem == (void *) -1)
		return 1;

	*out = (struct mas_execmem) {
		.mode = MAS_EXECMEM_W,
		.mem = mem,
		.size = size,
	};

	return 0;
}

void mas_execmem_free(struct mas_execmem *e)
{
	mas_execmem_set_mode(e, MAS_EXECMEM_W);

	munmap(e, e->size);
}

void mas_execmem_set_mode(struct mas_execmem *e, enum mas_execmem_mode mode)
{
	if (e->mode == mode)
		return;

#ifdef CURRENT_PLAT_thumb2
	e->mem = (char *) e->mem + (e->mode == MAS_EXECMEM_RE ? -1 : 0);
#endif

	assert2(mprotect(e->mem, e->size,
		mode == MAS_EXECMEM_RE ? PROT_READ | PROT_EXEC : PROT_WRITE) == 0,
		"could change the memory mode");

#ifdef CURRENT_PLAT_thumb2
	e->mem = (char *) e->mem + (mode == MAS_EXECMEM_RE ? 1 : -1);
#endif
}
