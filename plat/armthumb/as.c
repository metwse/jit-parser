#include "as.h"
#include "detail/opcodes.h"

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


void armthumb_as_xfeed(struct armthumb_as *as, const char *inst_str, ...)
{
	va_list ap;
	va_start(ap, inst_str);

	enum inst_opcode op = armthumb_consume_mnemonic(&inst_str);

	assert2((int) op != -1, "invalid mnemonic");

	struct inst_arg_tk arg_tks[MAX_PARAM_COUNT + 1];

	for (size_t i = 0; i < MAX_PARAM_COUNT + 1; i++) {
		struct inst_arg_tk arg_tk =
			armthumb_consume_arg_tk(&inst_str, i == 0);

		switch (arg_tk.ty) {
		case ARG_TK_REG_VA_STR:
			arg_tk.ty = ARG_TK_REG;
			arg_tk.seminfo.parsed.reg =
				armthumb_consume_arg_reg_tk(
					&(const char *) { va_arg(ap, char *) });
			break;

		case ARG_TK_REG_VA_NUM:
			arg_tk.ty = ARG_TK_REG;
			arg_tk.seminfo.parsed.reg = va_arg(ap, int);
			break;

		case ARG_TK_REG_LS_VA:
			arg_tk.ty = ARG_TK_REG_LS;
			arg_tk.seminfo.parsed.reg_ls.reg_ls = va_arg(ap, int);
			break;

		case ARG_TK_IMM_VA:
			arg_tk.ty = ARG_TK_IMM;
			arg_tk.seminfo.parsed.imm = va_arg(ap, int);
			break;

		default:
			break;
		}

		arg_tks[i] = arg_tk;
		if (arg_tks[i].ty == ARG_TK_END)
			break;
	}

	struct inst inst = armthumb_validate(op, arg_tks);

	armthumb_emit(as, inst);

	va_end(ap);
}
