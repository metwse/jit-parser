#include "detail/grammar.h"
#include "detail/static-init.h"
#include "detail/opcodes.h"

#include "../../src/common.h"

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define T char, enum inst_opcode, mnemonic_name
#include "../../vendor/libfun/include/hmap.h"

static struct fhmap_mnemonic_name mnemonic_name_map;


void thumb2_lexer_static_xinit(void)
{
	fhmap_mnemonic_name_xinit(&mnemonic_name_map);

	for (enum inst_opcode i = 0; i < INST_COUNT; i++)
		fhmap_mnemonic_name_xinsert(&mnemonic_name_map,
					    thumb2_mnemonics[i],
					    strlen(thumb2_mnemonics[i]),
					    &i);
}

void thumb2_lexer_static_destroy(void)
{
	fhmap_mnemonic_name_destroy(&mnemonic_name_map);
}

enum inst_opcode thumb2_consume_mnemonic(const char **inst)
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

uint32_t thumb2_consume_arg_reg_tk(const char **inst)
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

struct inst_arg_tk thumb2_consume_arg_tk(const char **inst, bool first)
{
	const char *start = *inst;

	if ((*inst)[0] == '\0')
		return (struct inst_arg_tk) { .ty = ARG_TK_END };

	/* cond suffix */
	if (isalpha(start[0]) && first) {
		if (strlen(start) >= 2 && isalpha(start[1]))
			for (size_t i = 0; i < COND_AL + 1; i++)
				if (start[0] == thumb2_mnemonic_cond_suffix[i][0] &&
				    start[1] == thumb2_mnemonic_cond_suffix[i][1]) {
					*inst += 2;
					return (struct inst_arg_tk) {
						.ty = ARG_TK_COND,
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

		return (struct inst_arg_tk) { .ty = ARG_TK_IMM, .seminfo.parsed.imm = imm };
	}
	case 'r':
	case 's':
	case 'l':
	case 'p': {  /* collect reg */
		uint32_t reg = thumb2_consume_arg_reg_tk(inst);

		if (reg < 16) {
			return (struct inst_arg_tk) {
				.ty = ARG_TK_REG,
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
			uint32_t reg = thumb2_consume_arg_reg_tk(inst);

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

		return (struct inst_arg_tk) {
			.ty = ARG_TK_REG_LS,
			.seminfo.parsed.reg_ls = reg_ls
		};
	}

	case '%':  /* vaarg placeholders */
		*inst += 2;  /* collect %x */
		switch ((*inst)[-1]) {
		case 'r':
			return (struct inst_arg_tk) { .ty = ARG_TK_REG_VA_NUM, };
		case 'R':
			return (struct inst_arg_tk) { .ty = ARG_TK_REG_VA_STR, };
		case 'l':
			return (struct inst_arg_tk) { .ty = ARG_TK_REG_LS_VA, };
		case 'i':
			return (struct inst_arg_tk) { .ty = ARG_TK_IMM_VA, };
		default:
			fatal("invalid vaarg placeholders %c", (*inst)[-1]);
			break;
		}
		break;

	case '[':  /* collect brace */
		(*inst)++;
		return (struct inst_arg_tk) { .ty = ARG_TK_BRACE };
	case '!':  /* excl */
		(*inst)++;
		return (struct inst_arg_tk) { .ty = ARG_TK_EXCL };
	case '\0':  /* end */
		return (struct inst_arg_tk) { .ty = ARG_TK_END };

	default:
		fatal("invalid character %c", (*inst)[0]);
		break;
	}

	_unreachable;
}
