/* The ARMv6 Thumb-2 assembler public methods. */

#ifndef MAS_THUMB2_AS_H
#define MAS_THUMB2_AS_H

#include "../../include/as-common.h"

#include <stdbool.h>
#include <stddef.h>

/* An ARMv6 Thumb-2 assembler instance. */
struct mas_thumb2_as;


/* Create a new assembler instance. */
struct mas_thumb2_as *mas_thumb2_as_xnew(enum mas_as_hint hints);

/* Feed an instruction or assembler directive to the assembler. */
void mas_thumb2_as_xfeed(struct mas_thumb2_as *as, const char *inst, ...);

/* Append the content of another assembler instance.
 *
 * The assembler instance whose contents are appended will be freed. If
 * isolate_scope set, labels of appended instance will not be visible to the
 * extended (parent) assembler. */
void mas_thumb2_as_xappend(struct mas_thumb2_as *as,
			   struct mas_thumb2_as *extend_with,
			   bool isolate_scope);

/* Calculate the size of the generated machine code. */
size_t mas_thumb2_as_output_size(const struct mas_thumb2_as *as);

/* Finalize the assembler and emit the generated macihe code.
 *
 * The assembler instance is freed. */
void mas_thumb2_as_finalize(struct mas_thumb2_as *as, void *out);


#endif
