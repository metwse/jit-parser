#include "as.h"
#include "encoding.h"

#include "../../include/common.h"
#include "../../osal/osal.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define T char, enum inst_opcode, mnemonic_name
#include "../../vendor/libfun/include/hmap.h"

#define cur ((uint16_t *) as->e.mem)[as->i]
#define inc as->i++


static struct fhmap_mnemonic_name mnemonic_name_map;


void armthumb_static_xinit(void)
{
	fhmap_mnemonic_name_xinit(&mnemonic_name_map);

	for (size_t i = 0; i < INST_COUNT; i++)
		fhmap_mnemonic_name_xinsert(&mnemonic_name_map,
					    mnemonics[i],
					    strlen(mnemonics[i]),
					    &i);
}

void armthumb_static_destroy(void)
{
	fhmap_mnemonic_name_destroy(&mnemonic_name_map);
}

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

static enum inst_opcode collect_mnemonic(const char **inst)
{
	size_t opcode_len = 0;
	for (; isalnum((*inst)[opcode_len]); opcode_len++)
		;

	const enum inst_opcode *opcode;

	for (int i = 0; i < 2; i++) {
		/* first try to match full alnum chars (mnemonic)
		 * if there's no match, try to remove last 2 letters (cond
		 * suffix) and try again. */
		opcode = fhmap_mnemonic_name_get(&mnemonic_name_map,
						 (*inst),
						 opcode_len);

		if (opcode != NULL) {
			(*inst) += opcode_len;
			return *opcode;
		}

		if (opcode_len > 2)
			opcode_len -= 2;
		else
			break;
	}

	return -1;
}

/* argument token */
struct tk {
	enum tk_ty {
		TK_REG, TK_REG_FMT_STR, TK_REG_FMT_NUM,
		TK_REG_LS, TK_REG_LS_FMT,
		TK_IMM, TK_IMM_FMT_NUM,
		TK_COND,
		TK_EXCL, TK_BRACE,
		TK_END,
	} ty;

	union tk_seminfo {
		struct token_data_argstr {
			const char *argstr;
			size_t argstr_len;
		} argstr;

		union inst_arg_data parsed;
	} seminfo;
};

static uint32_t collect_arg_reg(const char **inst)
{
	switch ((*inst)[0]) {
	case 'r': {
		(*inst)++;
		uint32_t reg = strtol(*inst, (char **) inst, 0);

		return reg;
	}
	case 's':
		if ((*inst)[1] == 'p') {
			*inst += 2;
			return 13;
		} else {
			return -1;
		}
	case 'l':
		if ((*inst)[1] == 'r') {
			*inst += 2;
			return 14;
		} else {
			return -1;
		}
	case 'p':
		if ((*inst)[1] == 'c') {
			*inst += 2;
			return 15;
		} else {
			return -1;
		}
	default:
		fatal("invalid character %c", (*inst)[0]);
	}

	_unreachable; return 0;
}

static struct tk collect_arg(const char **inst, bool first)
{
	const char *start = *inst;

	if ((*inst)[0] == '\0')
		return (struct tk) { .ty = TK_END };

	/* cond suffix */
	if (isalpha(start[0]) && first) {
		if (strlen(start) >= 2 && isalpha(start[1]))
			for (size_t i = 0; i < COND_AL + 1; i++)
				if (start[0] == mnemonic_cond_suffix[i][0] &&
				    start[1] == mnemonic_cond_suffix[i][1]) {
					*inst += 2;
					return (struct tk) {
						.ty = TK_COND,
						.seminfo.parsed.cond_suffix = i
					};
				}

		fatal("invalid condition suffix!");
	}

	while (isspace((*inst)[0]) || (*inst)[0] == ',' || (*inst)[0] == ']')
		(*inst)++;

	switch ((*inst)[0]) {
	case '#': {  /* collect imm */
		(*inst)++;
		uint32_t imm = strtol(*inst, (char **) inst, 0);

		return (struct tk) { .ty = TK_IMM, .seminfo.parsed.imm = imm };
	}
	case 'r':
	case 's':
	case 'l':
	case 'p': {  /* collect reg */
		uint32_t reg = collect_arg_reg(inst);

		if (reg < 16) {
			return (struct tk) {
				.ty = TK_REG,
				.seminfo.parsed.reg = reg
			};
		} else {
			fatal("invalid register number %d", reg); break;
		}
	}

	case '{': {  /* collect reg_ls */
		struct inst_arg_data_reg_ls reg_ls = { 0 };

		(*inst)++;  /* consume the { */
		while (isspace((*inst)[0]))
			(*inst)++;

		while ((*inst)[0] != '}') {
			uint32_t reg = collect_arg_reg(inst);

			while (isspace((*inst)[0]) || (*inst)[0] == ',')
				(*inst)++;

			if ((*inst)[0] == '\0')
				fatal("unterminated register list");

			if (!(reg < 8 || reg == 14 || reg == 15))
				fatal("only low registers and pc&lr allowed in "
				      "register list");
			if (reg < 8)
				reg_ls.reg_ls |= 1 << reg;
			if (reg == 14)
				reg_ls.lr = 1;
			if (reg == 15)
				reg_ls.pc = 1;
		}

		(*inst)++;  /* consume the } */

		return (struct tk) {
			.ty = TK_REG_LS,
			.seminfo.parsed.reg_ls = reg_ls
		};
	}

	case '[':  /* collect brace */
		(*inst)++;
		return (struct tk) { .ty = TK_BRACE };
	case '!':  /* excl */
		(*inst)++;
		return (struct tk) { .ty = TK_EXCL };
	case '\0':  /* end */
		return (struct tk) { .ty = TK_END };

	default:
		fatal("invalid character %c", start[0]);
		break;
	}

	_unreachable;
}

void armthumb_as_xfeed(struct armthumb_as *as, const char *inst, ...)
{
	va_list args;
	va_start(args, inst);

	enum inst_opcode op;
	struct tk inst_tokens[MAX_PARAM_COUNT + 1];

	op = collect_mnemonic(&inst);
	for (size_t i = 0; i < MAX_PARAM_COUNT + 1; i++) {
		inst_tokens[i] = collect_arg(&inst, i == 0);

		if (inst_tokens[i].ty == TK_END)
			break;
	}

	va_end(args);
}
