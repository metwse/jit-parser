/* Operating system abstraction layer, OSAL. */

#ifndef OSAL_H
#define OSAL_H

#include "../include/common.h"

#include <stddef.h>


/* Executable memory permissions. */
enum execmem_mode {
	EXECMEM_RW,  /* read-write */
	EXECMEM_E,  /* execute */
};

/* Executable memory. */
struct execmem {
	enum execmem_mode mode;
	void *mem;
	size_t size;
};


/* Allocates at least size bytes page-aligned execmem. Returned memory is not
 * executable yet. Returns non-zero in case of a failure. */
int execmem_alloc(struct execmem *out, size_t size) _wur;

/* Free the executable memory. */
void execmem_free(struct execmem *e);

/* Reallocates the executable memory with at least `size` bytes. Returns false
 * in case of a failure. */
int execmem_realloc(struct execmem *e, size_t size) _wur;

/* Set RW/E mode of execmem. */
void execmem_set_mode(struct execmem *e, enum execmem_mode mode);

/* Similar to execmem_alloc, but raises an erorr if memory allocation fails. */
static inline void execmem_xalloc(struct execmem *out, size_t size)
{
	assert2(execmem_alloc(out, size) == 0, "failed to allocate execmem");
}

/* Similar to execmem_realloc, but raises an erorr if memory allocation fails. */
static inline void execmem_xrealloc(struct execmem *e, size_t size)
{
	assert2(execmem_realloc(e, size) == 0, "failed to reallocate execmem");
}


#endif
