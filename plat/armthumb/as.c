#include "as.h"

#include "../../osal/osal.h"

#include <stdarg.h>
#include <stdint.h>

#define cur ((uint16_t *) as->e.mem)[as->i]
#define inc as->i++


void armthumb_as_xinit(struct armthumb_as *as)
{
	execmem_xalloc(&as->e, 1);
	fhmap_label_idx_map_xinit(&as->label_idx_map);
	as->i = 0;
}

struct execmem armthumb_as_finalize(struct armthumb_as *as)
{
	/* TODO: finalize label offsets */
	fhmap_label_idx_map_destroy(&as->label_idx_map);

	execmem_set_mode(&as->e, EXECMEM_E);

	return as->e;
}

void armthumb_as_xfeed(struct armthumb_as *as, const char *op, ...)
{
	va_list args;
	va_start(args, op);

	cur = 0x414F;
	inc;
	cur = 0x4480;
	inc;
	cur = 0x4407;

	va_end(args);
}
