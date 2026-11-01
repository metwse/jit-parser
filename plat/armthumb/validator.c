#include "detail/encoding.h"

#include "../../include/common.h"

#include <stdbool.h>


#define cur_param inst_params[res.opcode][res.alt_idx][i]


static bool validate_arg(const struct inst_arg_tk *arg_in,  /* argument to parse */
			   union inst_arg_data *arg_out,  /* parsed output */
			   enum inst_param param,  /* parameter of the argument */
			   const char **error)  /* optional error output*/
{
	*error = NULL;

	switch (arg_in->ty) {
	case ARG_TK_REG:
		if (!is_reg(param))
			return false;

		arg_out->reg = arg_in->seminfo.parsed.reg;
		break;

	case ARG_TK_REG_LS:
		if (!is_reg_ls(param))
			return false;

		arg_out->reg_ls = arg_in->seminfo.parsed.reg_ls;
		break;

	case ARG_TK_IMM:
		if (!is_imm(param))
			return false;

		arg_out->imm = arg_in->seminfo.parsed.imm;
		break;

	case ARG_TK_COND:
		if (param != PARAM_COND)
			return false;

		arg_out->cond_suffix = arg_in->seminfo.parsed.cond_suffix;
		break;

	case ARG_TK_REG_VA_STR:
	case ARG_TK_REG_VA_NUM:
	case ARG_TK_REG_LS_VA:
	case ARG_TK_IMM_VA:
		fatal("unexpected vaarg reference in parser");
		break;

	case ARG_TK_EXCL:
		return param == PARAM_EXCL;
		break;
	case ARG_TK_BRACE:
		return param == PARAM_BRACE;
		break;

	case ARG_TK_END:
		break;
	}

	return true;
}

struct inst armthumb_validate(enum inst_opcode op,
			      const struct inst_arg_tk *args)
{
	const char *errors[MAX_ALTERNATIVE_COUNT] = { 0 };
	struct inst res = { .opcode = op, .alt_idx = 0, };

	for (; res.alt_idx < MAX_ALTERNATIVE_COUNT; res.alt_idx++) {
		if (inst_params[res.opcode][res.alt_idx][0] == PARAME)
			break;

		for (size_t i = 0; i < MAX_PARAM_COUNT + 1; i++) {
			if (cur_param == PARAME)
				return res;

			if (!validate_arg(&args[i],
					  &res.data[i],
					  cur_param,
					  &errors[i]))
				break;
		}
	}

	fatal("invalid instruction");
	return (struct inst) { 0 };
}
