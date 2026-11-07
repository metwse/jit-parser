#include "as.h"
#include "detail/detail.h"
#include "detail/grammar.h"
#include "detail/opcodes.h"

#include "../../src/common.h"

#include <stdarg.h>
#include <stdint.h>


#define cur ((uint16_t *) as->e.mem)[as->i]
#define inc as->i++


/* TODO: hints */
struct mas_thumb2_as *mas_thumb2_as_xnew(enum mas_as_hint _unused hints)
{
	struct mas_thumb2_as *as = xmalloc(sizeof(struct mas_thumb2_as));

	fstack_asm_item_xinit(&as->asm);
	as->output_size = 0;

	fhmap_label_idx_map_xinit(&as->label_idx_map);
	as->up_as = NULL;

	return as;
}

void mas_thumb2_as_xfeed(struct mas_thumb2_as *as, const char *inst_str, ...)
{
	va_list ap;
	va_start(ap, inst_str);

	enum inst_opcode op = thumb2_consume_mnemonic(&inst_str);

	assert2((int) op != -1, "invalid mnemonic");

	struct inst_arg_tk arg_tks[MAX_PARAM_COUNT + 1];

	for (size_t i = 0; i < MAX_PARAM_COUNT + 1; i++) {
		struct inst_arg_tk arg_tk =
			thumb2_consume_arg_tk(&inst_str, i == 0);

		switch (arg_tk.ty) {
		case ARG_TK_REG_VA_STR:
			arg_tk.ty = ARG_TK_REG;
			arg_tk.seminfo.parsed.reg =
				thumb2_consume_arg_reg_tk(
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

	struct inst inst = thumb2_validate(op, arg_tks);

	fstack_asm_item_xpush(&as->asm, &(const struct asm_item) {
				     .ty = ASM_ITEM_INST,
				     .item.inst = inst
			      });
	as->output_size += thumb2_inst_size(&inst);

	va_end(ap);
}

void mas_thumb2_as_xappend(struct mas_thumb2_as *as,
			   struct mas_thumb2_as *extend_with,
			   bool isolate_scope)
{
	extend_with->up_as = isolate_scope ? NULL : as;

	fstack_asm_item_xpush(&as->asm, &(const struct asm_item) {
				     .ty = ASM_ITEM_AS,
				     .item.as = extend_with
			     });
	as->output_size += extend_with->output_size;
}

size_t mas_thumb2_as_output_size(const struct mas_thumb2_as *as)
{
	return as->output_size;
}

void mas_thumb2_as_finalize(struct mas_thumb2_as *as, void *out_)
{
	char *out = out_;

	for (size_t i = 0; i < fstack_asm_item_len(&as->asm); i++) {
		const struct asm_item *item = fstack_asm_item_at(&as->asm, i);

		if (item->ty == ASM_ITEM_INST) {
			thumb2_inst_emit(out, &item->item.inst);
			out += thumb2_inst_size(&item->item.inst);
		} else {
			mas_thumb2_as_finalize(item->item.as, out);
			out += item->item.as->output_size;
		}
	}

	fstack_asm_item_destroy(&as->asm);
	fhmap_label_idx_map_destroy(&as->label_idx_map);
	free(as);
}
