#include "as.h"
#include "detail/encoding.h"

#include "../../include/common.h"
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


void armthumb_as_xfeed(struct armthumb_as *as, const char *inst, ...)
{
	va_list args;
	va_start(args, inst);

	enum inst_opcode op = armthumb_consume_mnemonic(&inst);
	struct inst_arg_tk inst_tokens[MAX_PARAM_COUNT + 1];

	for (size_t i = 0; i < MAX_PARAM_COUNT + 1; i++) {
		inst_tokens[i] = armthumb_consume_arg_tk(&inst, i == 0);

		if (inst_tokens[i].ty == INST_ARG_TK_END)
			break;
	}

	va_end(args);
}
