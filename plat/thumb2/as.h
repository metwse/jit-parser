/* The ARMv6 Thumb-2 assembler public methods. */

#ifndef MAS_THUMB2_AS_H
#define MAS_THUMB2_AS_H

#include "../../osal/osal.h"

#define T size_t, _mas_label_idx
#include "../../vendor/libfun/include/stack.h"

#define T char, struct fstack__mas_label_idx, _mas_label_idx_map
#include "../../vendor/libfun/include/hmap.h"


/* The ARMv6 Thumb-2 assembler. */
struct mas_thumb2_as {
	struct execmem e;
	struct fhmap__mas_label_idx_map label_idx_map;
	size_t i;
};


/* Initialize a new assembler. */
void mas_thumb2_as_xinit(struct mas_thumb2_as *);

/* Push a new instruction */
void mas_thumb2_as_xfeed(struct mas_thumb2_as *, const char *, ...);

/* Finalize the assembler. */
struct execmem mas_thumb2_as_finalize(struct mas_thumb2_as *);


#endif
