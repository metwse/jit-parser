/* The ARMv6 thumb (16 bit) instruction set assembler. */

#ifndef ARMTHUMB_OPCODES_H
#define ARMTHUMB_OPCODES_H

#include "../../osal/osal.h"

#define T size_t, label_idx
#include "../../vendor/libfun/include/stack.h"

#define T char, struct fstack_label_idx, label_idx_map
#include "../../vendor/libfun/include/hmap.h"


/* The ARMv6 thumb (16 bit) assembler. */
struct armthumb_as {
	struct execmem e;
	struct fhmap_label_idx_map label_idx_map;
	size_t i;
};


/* Initialize static variables for the ARM thumb assembler. */
void armthumb_static_xinit(void);

/* Free the resources owned by static variables of the ARM thumb assembler. */
void armthumb_static_destroy(void);

/* Initialize a new assembler. */
void armthumb_as_xinit(struct armthumb_as *);

/* Push a new instruction */
void armthumb_as_xfeed(struct armthumb_as *, const char *, ...);

/* Finalize the assembler. */
struct execmem armthumb_as_finalize(struct armthumb_as *);


#endif
