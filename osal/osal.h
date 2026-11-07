/* Operating system abstraction layer, OSAL. */

#ifndef OSAL_H
#define OSAL_H

#include "../include/detail.h"

#include <assert.h>
#include <stddef.h>


/* Executable memory permissions. */
enum mas_execmem_mode {
	MAS_EXECMEM_W,  /* write */
	MAS_EXECMEM_RE,  /* read-execute */
};

/* Executable memory. */
struct mas_execmem {
	enum mas_execmem_mode mode;

	union {
		void *mem;
		void (*execmem)(void);
	};

	size_t size;
};


/* Allocates at least size bytes page-aligned execmem. Returned memory is not
 * executable yet. Returns non-zero in case of a failure. */
int mas_execmem_alloc(struct mas_execmem *out, size_t size) _mas_wur;

/* Free the executable memory. */
void mas_execmem_free(struct mas_execmem *e);

/* Set W/RE mode of execmem. */
void mas_execmem_set_mode(struct mas_execmem *e, enum mas_execmem_mode mode);

/* Similar to mas_execmem_alloc, but raises an erorr if memory allocation fails. */
static inline void mas_execmem_xalloc(struct mas_execmem *out, size_t size)
{
	assert(mas_execmem_alloc(out, size) == 0 && "failed to allocate execmem");
}


#endif
