/* OP encoding. For internal use only. */

#ifndef ARMTHUMB_OPCODES_H
#define ARMTHUMB_OPCODES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct armthumb_as;  /* defined in ../as.h */


#define MAX_PARAM_COUNT 4
#define MAX_ALTERNATIVE_COUNT 9
#define INST_COUNT (OP_UDF + 1)

#define is_reg(param) \
	((param) >= PARAM_LOW_REG && (param) <= PARAM_PC)
#define is_reg_ls(param) \
	((param) >= PARAM_LOW_REG_LS && (param) <= PARAM_PUSH_REG_LS)
#define is_imm(param) \
	((param) >= PARAM_IMM0 && (param) <= PARAM_IMM24_0_LABEL)


enum inst_opcode {
	OP_ADCS, OP_ADDS, OP_ADD, OP_ANDS, OP_ASRS, OP_B,
	OP_BICS, OP_BKPT, OP_BL, OP_BLX, OP_BX, OP_CMN, OP_CMP,
	OP_EORS, OP_LDM, OP_LDR, OP_LDRB, OP_LDRH, OP_LDRSB, OP_LDRSH,
	OP_LSLS, OP_LSRS, OP_MOV, OP_MOVS, OP_MULS, OP_MVNS, OP_NEGS, OP_NOP,
	OP_ORRS, OP_POP, OP_PUSH, OP_RORS, OP_RSBS, OP_SBCS, OP_SEV, OP_STM,
	OP_STR, OP_STRB, OP_STRH, OP_SUBS, OP_SUB, OP_TST, OP_UDF
};

/* Conditional execution flags. */
enum inst_cond {
	COND_EQ, COND_NE, COND_CS, COND_CC, COND_MI, COND_PL, COND_VS, COND_VC,
	COND_HI, COND_LS, COND_GE, COND_LT, COND_GT, COND_LE, COND_AL,
};

/* Instruction parameters. */
enum inst_param {
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


static const char *const mnemonics[INST_COUNT] = {
	"adcs", "adds", "add", "ands", "asrs", "b",
	"bics", "bkpt", "bl", "blx", "bx", "cmn", "cmp",
	"eors", "ldm", "ldr", "ldrb", "ldrh", "ldrsb", "ldrsh",
	"lsls", "lsrs", "mov", "movs", "muls", "mvns", "negs", "nop",
	"orrs", "pop", "push", "rors", "rsbs", "sbcs", "sev", "stm",
	"str", "strb", "strh", "subs", "sub", "tst", "udf"
};

static const char *const mnemonic_cond_suffix[] = {
	"eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc",
	"hi", "ls", "ge", "lt", "gt", "le", "al",
};

/* Human-readable parameter names (for debug/grammar dump). */
static const char *const param_names[] = {
	"",

	"r0-r7", "r8-r14", "r0-r15", "r0-r13,r15", "r0-r14", "sp", "pc",

	"{ r0-r7 }", "{ r0-r7, pc }", "{ r0-r7, lr }",

	"#imm3",
	"#imm5", "#imm5_00",
	"#imm7_00",
	"#imm8", "#imm8_0", "#imm8_0_label", "#imm8_00", "#imm8_00_label",
	"#imm11_0_label",
	"#imm24_0_label",

	"!",
	"[",
	"<cond>",
	""
};


/* Instructions are represented as:
 * <mnemonic><mnemonic_cond_suffix> <parameters...>
 *
 * An instruction can have conditional execution mode IF its first parameter
 * is PARAM_COND. */

static const enum inst_param
inst_params[INST_COUNT][MAX_ALTERNATIVE_COUNT + 1][MAX_PARAM_COUNT + 1] = {
			/* register */
	[OP_ADCS] = {	{ PARAM_LOW_REG, PARAM_LOW_REG } },

			/* immediate */
	[OP_ADDS] = {	{ PARAM_LOW_REG, PARAM_LOW_REG, PARAM_IMM3 } ,
			{ PARAM_LOW_REG, PARAM_IMM8 },
			/* register */
			{ PARAM_LOW_REG, PARAM_LOW_REG, PARAM_LOW_REG } },

	[OP_ADD] = {	{ PARAM_NOT_SP_REG, PARAM_ANY_REG },
			/* SP plus immediate */
			{ PARAM_LOW_REG, PARAM_SP, PARAM_IMM8_00 },
			{ PARAM_SP, PARAM_IMM7_00 },
			/* SP plus register */
			{ PARAM_ANY_REG, PARAM_SP },
			{ PARAM_SP, PARAM_ANY_REG },
			{ PARAM_LOW_REG, PARAM_PC, PARAM_IMM8_00 } },

			/* register */
	[OP_ANDS] = {	{ PARAM_LOW_REG, PARAM_LOW_REG } },

			/* immediate */
	[OP_ASRS] = {	{ PARAM_LOW_REG, PARAM_LOW_REG, PARAM_IMM5 },
			/* register */
			{ PARAM_LOW_REG, PARAM_LOW_REG } },

	[OP_B] = {	{ PARAM_COND, PARAM_IMM8_0_LABEL },			/* TODO: make this label signed (-256-254) */
			{ PARAM_IMM11_0_LABEL } },				/* TODO: make this label signed (-2048-2046) */

			/* register */
	[OP_BICS] = {	{ PARAM_LOW_REG, PARAM_LOW_REG } },

	[OP_BKPT] = {	{ PARAM_IMM8 } },

	[OP_BL] = {	{ PARAM_IMM24_0_LABEL } },				/* TODO: make this label signed (-16777216-16777214) */

			/* register */
	[OP_BLX] = {	{ PARAM_NOT_PC_REG } },

	[OP_BX] = {	{ PARAM_NOT_PC_REG } },

			/* register */
	[OP_CMN] = {	{ PARAM_LOW_REG, PARAM_LOW_REG } },
			/* immediate */
	[OP_CMP] = {	{ PARAM_LOW_REG, PARAM_IMM8 },
			/* register */
			{ PARAM_LOW_REG, PARAM_LOW_REG },
			{ PARAM_ANY_REG, PARAM_ANY_REG } },

			/* register */
	[OP_EORS] = {	{ PARAM_LOW_REG, PARAM_LOW_REG } },

			/* load */
	[OP_LDM] = {	{ PARAM_LOW_REG, PARAM_LOW_REG_LS },
			{ PARAM_LOW_REG, PARAM_EXCL, PARAM_LOW_REG_LS }, },
			/* immediate */
	[OP_LDR] = {	{ PARAM_LOW_REG, PARAM_LOW_REG },
			{ PARAM_LOW_REG, PARAM_BRACE, PARAM_LOW_REG, PARAM_IMM5_00 },
			{ PARAM_LOW_REG, PARAM_SP },
			{ PARAM_LOW_REG, PARAM_BRACE, PARAM_SP, PARAM_IMM8_00 },
			/* literal */
			{ PARAM_LOW_REG, PARAM_IMM8_00_LABEL },
			{ PARAM_LOW_REG, PARAM_BRACE, PARAM_PC, PARAM_IMM8_00 },
			/* register */
			{ PARAM_LOW_REG, PARAM_BRACE, PARAM_LOW_REG, PARAM_LOW_REG } },

			/* immediate */
	[OP_LDRB] = {	{ PARAM_LOW_REG, PARAM_LOW_REG },
			{ PARAM_LOW_REG, PARAM_BRACE, PARAM_LOW_REG, PARAM_IMM5 },
			/* register */
			{ PARAM_LOW_REG, PARAM_BRACE, PARAM_LOW_REG, PARAM_LOW_REG } },
			/* immediate */
	[OP_LDRH] = {	{ PARAM_LOW_REG, PARAM_LOW_REG },
			{ PARAM_LOW_REG, PARAM_BRACE, PARAM_LOW_REG, PARAM_IMM5 },
			/* register */
			{ PARAM_LOW_REG, PARAM_BRACE, PARAM_LOW_REG, PARAM_LOW_REG } },

	[OP_LDRSB] = {	{ PARAM_LOW_REG, PARAM_BRACE, PARAM_LOW_REG, PARAM_LOW_REG } },
	[OP_LDRSH] = {	{ PARAM_LOW_REG, PARAM_BRACE, PARAM_LOW_REG, PARAM_LOW_REG } },

			/* immediate */
	[OP_LSLS] = {	{ PARAM_LOW_REG, PARAM_LOW_REG, PARAM_IMM5 },
			/* register */
			{ PARAM_LOW_REG, PARAM_LOW_REG } },
			/* immediate */
	[OP_LSRS] = {	{ PARAM_LOW_REG, PARAM_LOW_REG, PARAM_IMM5 },
			/* register */
			{ PARAM_LOW_REG, PARAM_LOW_REG } },

	[OP_MOV] = {	{ PARAM_ANY_REG, PARAM_ANY_REG } },
			/* immediate */
	[OP_MOVS] = {	{ PARAM_LOW_REG, PARAM_IMM8 },
			/* register */
			{ PARAM_LOW_REG, PARAM_LOW_REG } },

	[OP_MULS] = {	{ PARAM_LOW_REG, PARAM_LOW_REG } },

			/* register */
	[OP_MVNS] = {	{ PARAM_LOW_REG, PARAM_LOW_REG } },

	[OP_NEGS] = {	{ PARAM_LOW_REG, PARAM_LOW_REG } },

	[OP_NOP] = { 	{ PARAM_NOPARAM } },

			/* register */
	[OP_ORRS] = {	{ PARAM_LOW_REG, PARAM_LOW_REG } },

	[OP_POP] = {	{ PARAM_POP_REG_LS } },
	[OP_PUSH] = {	{ PARAM_PUSH_REG_LS } },

			/* register */
	[OP_RORS] = {	{ PARAM_LOW_REG, PARAM_LOW_REG } },

			/* register */
	[OP_RSBS] = {	{ PARAM_LOW_REG, PARAM_LOW_REG },
			{ PARAM_LOW_REG, PARAM_LOW_REG, PARAM_IMM0 } },
	[OP_SBCS] = {	{ PARAM_LOW_REG, PARAM_LOW_REG } },

	[OP_SEV] = { 	{ PARAM_NOPARAM } },

			/* store */
	[OP_STM] = {	{ PARAM_LOW_REG, PARAM_EXCL, PARAM_LOW_REG_LS } },
			/* immediate */
	[OP_STR] = {	{ PARAM_LOW_REG, PARAM_LOW_REG },
			{ PARAM_LOW_REG, PARAM_BRACE, PARAM_LOW_REG, PARAM_IMM5 },
			{ PARAM_LOW_REG, PARAM_SP },
			{ PARAM_LOW_REG, PARAM_BRACE, PARAM_SP, PARAM_IMM8_00 },
			/* register */
			{ PARAM_LOW_REG, PARAM_BRACE, PARAM_LOW_REG, PARAM_LOW_REG } },

			/* immediate */
	[OP_STRB] = {	{ PARAM_LOW_REG, PARAM_LOW_REG },
			{ PARAM_LOW_REG, PARAM_BRACE, PARAM_LOW_REG, PARAM_IMM5 },
			/* register */
			{ PARAM_LOW_REG, PARAM_BRACE, PARAM_LOW_REG, PARAM_LOW_REG } },
			/* immediate */
	[OP_STRH] = {	{ PARAM_LOW_REG, PARAM_LOW_REG },
			{ PARAM_LOW_REG, PARAM_BRACE, PARAM_LOW_REG, PARAM_IMM5 },
			/* register */
			{ PARAM_LOW_REG, PARAM_BRACE, PARAM_LOW_REG, PARAM_LOW_REG } },

			/* immediate */
	[OP_SUBS] = {	{ PARAM_LOW_REG, PARAM_LOW_REG, PARAM_IMM3 },
			{ PARAM_LOW_REG, PARAM_IMM8 },
			/* register */
			{ PARAM_LOW_REG, PARAM_LOW_REG, PARAM_LOW_REG } },
			/* SP minus immediate */
	[OP_SUB] = {	{ PARAM_SP, PARAM_SP, PARAM_IMM7_00 } },

			/* register */
	[OP_TST] = {	{ PARAM_LOW_REG, PARAM_LOW_REG } },

	[OP_UDF] = {	{ PARAM_IMM8 },
			{ PARAM_NOPARAM } },
};


/* Prints assembler grammar. */
void armthumb_dump_grammar(void);

/* Extract opcode from the string, consuming its mnemonic. Returns -1 if
 * the mnemonic is not valid.*/
enum inst_opcode armthumb_consume_mnemonic(const char **inst);

/* Extract one argument from instruction string.
 * Checks condition suffix if first==true. */
struct inst_arg_tk armthumb_consume_arg_tk(const char **inst, bool first);

/* Extract one register argument from instruction string. */
uint32_t armthumb_consume_arg_reg_tk(const char **inst);

/* Parse and validate the token list. */
struct inst armthumb_validate(enum inst_opcode, const struct inst_arg_tk *);

/* Emit instruction. */
void armthumb_emit(struct armthumb_as *, struct inst);


#endif
