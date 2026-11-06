/* OP encoding. For internal use only. */

#ifndef THUMB2_OPCODES_H
#define THUMB2_OPCODES_H

#include "../../include/detail.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct mas_thumb2_as;  /* defined in ../as.h */


#define MAX_PARAM_COUNT 4
#define MAX_ALTERNATIVE_COUNT 9

/* (Number of opcodes - ORDER OP) */
#define INST_COUNT (OP_UDF + 1)
/* (Number of condition suffixes - ORDER COND) */
#define COND_SUFFIX_COUNT (COND_AL + 1)

/* (Number of parameter kinds and their reg, reg_ls, imm ranges - ORDER PARAM) */
#define PARAM_KIND_COUNT (PARAM_NOPARAM + 1)
#define is_reg(param) \
	((param) >= PARAM_LOW_REG && (param) <= PARAM_PC)
#define is_reg_ls(param) \
	((param) >= PARAM_LOW_REG_LS && (param) <= PARAM_PUSH_REG_LS)
#define is_imm(param) \
	((param) >= PARAM_IMM0 && (param) <= PARAM_IMM24_0_LABEL)


enum inst_opcode {  // GREP "ORDER OP" if you change this enum
	OP_ADCS, OP_ADDS, OP_ADD, OP_ANDS, OP_ASRS, OP_B,
	OP_BICS, OP_BKPT, OP_BL, OP_BLX, OP_BX, OP_CMN, OP_CMP,
	OP_EORS, OP_LDM, OP_LDR, OP_LDRB, OP_LDRH, OP_LDRSB, OP_LDRSH,
	OP_LSLS, OP_LSRS, OP_MOV, OP_MOVS, OP_MULS, OP_MVNS, OP_NEGS, OP_NOP,
	OP_ORRS, OP_POP, OP_PUSH, OP_RORS, OP_RSBS, OP_SBCS, OP_SEV, OP_STM,
	OP_STR, OP_STRB, OP_STRH, OP_SUBS, OP_SUB, OP_TST, OP_UDF
};

/* Conditional execution flags. */
enum inst_cond {  // GREP "ORDER COND" if you change this enum
	COND_EQ, COND_NE, COND_CS, COND_CC, COND_MI, COND_PL, COND_VS, COND_VC,
	COND_HI, COND_LS, COND_GE, COND_LT, COND_GT, COND_LE, COND_AL,
};

/* Instruction parameters. */
enum inst_param {  // GREP "ORDER PARAM" if you change this enum
	PARAME = 0,  /* end of argument list */

	PARAM_LOW_REG,  /* r0-r7 */
	PARAM_HIGH_NOT_SP_REG,  /* r8-r14 */
	PARAM_ANY_REG,  /* r0-r15 */
	PARAM_NOT_SP_REG,  /* r0-r13,r15 */
	PARAM_NOT_PC_REG,  /* r0-r14 */
	PARAM_SP,  /* r13 */
	PARAM_PC,  /* r15 */

	PARAM_LOW_REG_LS,  /* { r0-r7 } */
	PARAM_POP_REG_LS,  /* { r0-r7, pc } */
	PARAM_PUSH_REG_LS,  /* { r0-r7, lr } */

	PARAM_IMM0,  /* constant 0 */
	PARAM_IMM3,  /* 3-bit imm */
	PARAM_IMM5, PARAM_IMM5_00,
	PARAM_IMM7_00,
	PARAM_IMM8, PARAM_IMM8_0, PARAM_IMM8_0_LABEL,
		PARAM_IMM8_00, PARAM_IMM8_00_LABEL,
	PARAM_IMM11_0_LABEL,
	PARAM_IMM24_0_LABEL,

	PARAM_EXCL,
	PARAM_BRACE,
	PARAM_COND,
	PARAM_NOPARAM,
};

/* Parsed instruction. */
struct inst {
	enum inst_opcode opcode;

	size_t alt_idx;  /* alternative index in format table */

	union inst_arg_data {
		enum inst_arg_data_reg {
			ARG_REG_R0, ARG_REG_R1, ARG_REG_R2, ARG_REG_R3,
			ARG_REG_R4, ARG_REG_R5, ARG_REG_R6, ARG_REG_R7,
			ARG_REG_R8, ARG_REG_R9, ARG_REG_R10, ARG_REG_R11,
			ARG_REG_R12, ARG_REG_R13, ARG_REG_R14, ARG_REG_R15,
		} reg;

		struct inst_arg_data_reg_ls {
			union {
				struct {
				uint32_t r0 : 1, r1 : 1, r2 : 1, r3 : 1,
					 r4 : 1, r5 : 1, r6 : 1, r7 : 1,
					 pc : 1, lr : 1;
				};

				uint32_t reg_ls;
			};
		} reg_ls;

		uint32_t imm;

		enum inst_cond cond_suffix;
	} args[MAX_PARAM_COUNT];
};

/* Argument token.  */
struct inst_arg_tk {
	enum inst_arg_tk_ty {
		ARG_TK_REG,  /* argument is register */
		ARG_TK_REG_VA_STR,  /* read register string from vaargs */
		ARG_TK_REG_VA_NUM,  /* read register number from vaargs */
		ARG_TK_REG_LS,  /* the argument is register list */
		ARG_TK_REG_LS_VA,  /* read register list from vaargs */
		ARG_TK_IMM,  /* the argument is imm */
		ARG_TK_IMM_VA,  /* read imm from vaargs */
		ARG_TK_COND,  /* the argument is execution cond flag */
		ARG_TK_EXCL,  /* used for parsing */
		ARG_TK_BRACE,  /* used for parsing */
		ARG_TK_END,
	} ty;

	union inst_arg_tk_seminfo {
		struct {
			const char *argstr;
			size_t argstr_len;
		} argstr;

		union inst_arg_data parsed;
	} seminfo;
};

_masi const char *const thumb2_mnemonics[INST_COUNT];
_masi const char *const thumb2_mnemonic_cond_suffix[COND_SUFFIX_COUNT];

/* Human-readable parameter names (for debug/grammar dump). */
_masi const char *const thumb2_param_names[PARAM_KIND_COUNT];

/* Instruction-parameter table, for assembly syntax. */
_masi const enum inst_param
thumb2_inst_params[INST_COUNT][MAX_ALTERNATIVE_COUNT + 1][MAX_PARAM_COUNT + 1];


/* Prints assembler grammar, for development debug. */
_masi void thumb2_dump_grammar(void);

/* Extract opcode from the string, consuming its mnemonic. Returns -1 if
 * the mnemonic is not valid.*/
_masi enum inst_opcode thumb2_consume_mnemonic(const char **inst);

/* Extract one argument from instruction string.
 * Checks condition suffix if first==true. */
_masi struct inst_arg_tk thumb2_consume_arg_tk(const char **inst, bool first);

/* Extract one register argument from instruction string. */
_masi uint32_t thumb2_consume_arg_reg_tk(const char **inst);

/* Parse and validate the token list. */
_masi struct inst thumb2_validate(enum inst_opcode, const struct inst_arg_tk *);

/* Emit instruction. */
_masi void thumb2_emit(struct mas_thumb2_as *, struct inst);


#endif
