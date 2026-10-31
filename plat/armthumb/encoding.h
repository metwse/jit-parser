/* OP encoding. For internal use only. */

#ifndef ARMTHUMB_ENCODING_H
#define ARMTHUMB_ENCODING_H

#include <stdbool.h>
#include <stddef.h>


#define MAX_ARG_COUNT 4
#define MAX_ALTERNATIVE_COUNT 9
#define INST_COUNT (OP_UDF + 1)

#define is_reg(arg) ((arg) >= ARG_LOW_REG && (arg) <= ARG_PC)
#define is_reg_ls(arg) ((arg) >= ARG_LOW_REG_LS && (arg) <= ARG_PUSH_REG_LS)
#define is_imm(arg) ((arg) >= ARG_IMM3 && (arg) <= ARG_IMM24_0_LABEL)


/* Instruction format. */
struct inst_encoding {
	enum inst_mnemonic {
		OP_ACDS, OP_ADDS, OP_ADD, OP_ADR, OP_ANDS, OP_ASRS, OP_B,
		OP_BICS, OP_BKPT, OP_BL, OP_BLX, OP_BX, OP_CMN, OP_CMP,
		OP_EORS, OP_LDM, OP_LDR, OP_LDRB, OP_LDRH, OP_LDRSB, OP_LDRSH,
		OP_LSLS, OP_LSRS, OP_MOVS, OP_MULS, OP_MVNS, OP_NEG, OP_NOP,
		OP_ORRS, OP_POP, OP_PUSH, OP_RORS, OP_RSBS, OP_SBCS, OP_STM,
		OP_STR, OP_STRB, OP_STRH, OP_SUBS, OP_SUB, OP_TST, OP_UDF
	} mnemonic;

	enum inst_arg {
		ARGE = 0,  /* end of argument list */

		ARG_LOW_REG,  /* r0-r7 */
		ARG_HIGH_NOT_SP_REG,  /* r8-r14 */
		ARG_ANY_REG,  /* r0-r15 */
		ARG_NOT_SP_REG,  /* r0-r13,r15 */
		ARG_NOT_PC_REG,  /* r0-r14 */
		ARG_SP,  /* r13 */
		ARG_PC,  /* r15 */

		ARG_LOW_REG_LS,  /* { r0-r7 } */
		ARG_POP_REG_LS,  /* { r0-r7, pc } */
		ARG_PUSH_REG_LS,  /* { r0-r7, lr } */

		ARG_IMM3,  /* 3-bit imm */
		ARG_IMM5, ARG_IMM5_00,
		ARG_IMM7_00,
		ARG_IMM8, ARG_IMM8_0, ARG_IMM8_0_LABEL, ARG_IMM8_00, ARG_IMM8_00_LABEL,
		ARG_IMM11_0_LABEL,
		ARG_IMM24_0_LABEL,

		ARG_EXCL,
		ARG_BRACE,
		ARG_COND,
		ARG_NOARG,
	} args[MAX_ARG_COUNT + 1];
};


/* Conditional execution flags. */
enum inst_cond {
	EQ, NE, CS, CC, MI, PL, VS, VC, HI, LS, GE, LT, GT, LE, AL,
};


static const char *const mnemonic_names[INST_COUNT] = {
	"acds", "adds", "add", "adr", "ands", "asrs", "b",
	"bics", "bkpt", "bl", "blx", "bx", "cmn", "cmp",
	"eors", "ldm", "ldr", "ldrb", "ldrh", "ldrsb", "ldrsh",
	"lsls", "lsrs", "movs", "muls", "mvns", "neg", "nop",
	"orrs", "pop", "push", "rors", "rsbs", "sbcs", "stm",
	"str", "strb", "strh", "subs", "sub", "tst", "udf"
};

static const char *const arg_names[] = {
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

static const char *const cond_names[] = {
	"eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc",
	"hi", "ls", "ge", "lt", "gt", "le", "al",
};


static const enum inst_arg inst_args[INST_COUNT][MAX_ALTERNATIVE_COUNT + 1][MAX_ARG_COUNT + 1] = {
			/* register */
	[OP_ACDS] = {	{ ARG_LOW_REG, ARG_LOW_REG } },

			/* immediate */
	[OP_ADDS] = {	{ ARG_LOW_REG, ARG_LOW_REG, ARG_IMM3 } ,
			{ ARG_LOW_REG, ARG_IMM8 },
			/* register */
			{ ARG_LOW_REG, ARG_LOW_REG, ARG_LOW_REG } },

	[OP_ADD] = {	{ ARG_NOT_SP_REG, ARG_ANY_REG },
			/* SP plus immediate */
			{ ARG_ANY_REG, ARG_SP, ARG_IMM8_00 },
			{ ARG_SP, ARG_SP, ARG_IMM7_00 },
			/* SP plus register */
			{ ARG_ANY_REG, ARG_SP },
			{ ARG_SP, ARG_ANY_REG },
			{ ARG_LOW_REG, ARG_PC, ARG_IMM8_00 } },

	[OP_ADR] = {	{ ARG_LOW_REG, ARG_IMM8_00_LABEL } },

			/* register */
	[OP_ANDS] = {	{ ARG_LOW_REG, ARG_LOW_REG } },

			/* immediate */
	[OP_ASRS] = {	{ ARG_LOW_REG, ARG_LOW_REG, ARG_IMM5 },
			/* register */
			{ ARG_LOW_REG, ARG_LOW_REG } },

	[OP_B] = {	{ ARG_COND, ARG_IMM8_0_LABEL },
			{ ARG_IMM11_0_LABEL } },

			/* register */
	[OP_BICS] = {	{ ARG_LOW_REG, ARG_LOW_REG } },

	[OP_BKPT] = {	{ ARG_IMM8 } },

	[OP_BL] = {	{ ARG_IMM24_0_LABEL } },

			/* register */
	[OP_BLX] = {	{ ARG_NOT_PC_REG } },

	[OP_BX] = {	{ ARG_NOT_PC_REG } },

			/* register */
	[OP_CMN] = {	{ ARG_LOW_REG, ARG_LOW_REG } },
			/* immediate */
	[OP_CMP] = {	{ ARG_LOW_REG, ARG_IMM8 },
			/* register */
			{ ARG_LOW_REG, ARG_LOW_REG },
			{ ARG_HIGH_NOT_SP_REG, ARG_HIGH_NOT_SP_REG } },

	/* register */
	[OP_EORS] = {	{ ARG_LOW_REG, ARG_LOW_REG } },

	/* load */
	[OP_LDM] = {	{ ARG_LOW_REG, ARG_LOW_REG_LS },
			{ ARG_LOW_REG, ARG_EXCL, ARG_LOW_REG_LS }, },
			/* immediate */
	[OP_LDR] = {	{ ARG_LOW_REG, ARG_LOW_REG },
			{ ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_IMM5 },
			{ ARG_LOW_REG, ARG_SP },
			{ ARG_LOW_REG, ARG_BRACE, ARG_SP, ARG_IMM8_00 },
			/* literal */
			{ ARG_LOW_REG, ARG_IMM8_00_LABEL },
			{ ARG_LOW_REG, ARG_BRACE, ARG_PC, ARG_IMM8_00 },
			/* register */
			{ ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_LOW_REG } },

	/* immediate */
	[OP_LDRB] = {	{ ARG_LOW_REG, ARG_LOW_REG },
			{ ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_IMM5 },
			/* register */
			{ ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_LOW_REG } },
			/* immediate */
	[OP_LDRH] = {	{ ARG_LOW_REG, ARG_LOW_REG },
			{ ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_IMM5 },
			/* register */
			{ ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_LOW_REG } },

	[OP_LDRSB] = {	{ ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_LOW_REG } },
	[OP_LDRSH] = {	{ ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_LOW_REG } },

			/* immediate */
	[OP_LSLS] = {	{ ARG_LOW_REG, ARG_LOW_REG, ARG_IMM5 },
			/* register */
			{ ARG_LOW_REG, ARG_LOW_REG } },
			/* immediate */
	[OP_LSRS] = {	{ ARG_LOW_REG, ARG_LOW_REG, ARG_IMM5 },
			/* register */
			{ ARG_LOW_REG, ARG_LOW_REG } },

			/* immediate */
	[OP_MOVS] = {	{ ARG_LOW_REG, ARG_IMM8 },
			/* register */
			{ ARG_LOW_REG, ARG_LOW_REG },
			{ ARG_ANY_REG, ARG_ANY_REG } },

	[OP_MULS] = {	{ ARG_LOW_REG, ARG_LOW_REG } },

			/* register */
	[OP_MVNS] = {	{ ARG_LOW_REG, ARG_LOW_REG } },

	[OP_NEG] = {	{ ARG_LOW_REG, ARG_LOW_REG } },

	[OP_NOP] = { 	{ ARG_NOARG } },

			/* register */
	[OP_ORRS] = {	{ ARG_LOW_REG, ARG_LOW_REG } },

	[OP_POP] = {	{ ARG_POP_REG_LS } },
	[OP_PUSH] = {	{ ARG_PUSH_REG_LS } },

	/* register */
	[OP_RORS] = {	{ ARG_LOW_REG, ARG_LOW_REG } },

	/* register */
	[OP_RSBS] = {	{ ARG_LOW_REG, ARG_LOW_REG } },
	[OP_SBCS] = {	{ ARG_LOW_REG, ARG_LOW_REG } },

	/* store */
	[OP_STM] = {	{ ARG_LOW_REG, ARG_LOW_REG_LS },
			{ ARG_LOW_REG, ARG_EXCL, ARG_LOW_REG_LS }, },
			/* immediate */
	[OP_STR] = {	{ ARG_LOW_REG, ARG_LOW_REG },
			{ ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_IMM5 },
			{ ARG_LOW_REG, ARG_SP },
			{ ARG_LOW_REG, ARG_BRACE, ARG_SP, ARG_IMM8_00 },
			/* register */
			{ ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_LOW_REG } },

			/* immediate */
	[OP_STRB] = {	{ ARG_LOW_REG, ARG_LOW_REG },
			{ ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_IMM5 },
			/* register */
			{ ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_LOW_REG } },
			/* immediate */
	[OP_STRH] = {	{ ARG_LOW_REG, ARG_LOW_REG },
			{ ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_IMM5 },
			/* register */
			{ ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_LOW_REG } },

			/* immediate */
	[OP_SUBS] = {	{ ARG_LOW_REG, ARG_LOW_REG, ARG_IMM3 },
			{ ARG_LOW_REG, ARG_IMM8 },
			/* register */
			{ ARG_LOW_REG, ARG_LOW_REG, ARG_LOW_REG } },
			/* SP minus immediate */
	[OP_SUB] = {	{ ARG_SP, ARG_SP, ARG_IMM7_00 } },

	/* register */
	[OP_TST] = {	{ ARG_LOW_REG, ARG_LOW_REG } },

	[OP_UDF] = {	{ ARG_IMM8 } },
};


void armthumb_dump_grammar(void);


#endif
