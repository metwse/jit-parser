/* OP encoding. For internal use only. */

#ifndef ARMTHUMB_ENCODING_H
#define ARMTHUMB_ENCODING_H

#include <stdbool.h>


#define is_reg(arg) ((arg) >= ARG_LOW_REG && (arg) <= ARG_PC)
#define is_reg_ls(arg) ((arg) >= ARG_LOW_REG_LS && (arg) <= ARG_PUSH_REG_LS)
#define is_imm(arg) ((arg) >= ARG_IMM3 && (arg) <= ARG_IMM24_0_LABEL)

struct inst_encoding {
	const char *mnemonic;
	bool has_cond;
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
	} args[5];
};

/* Conditional execution flags. */
enum inst_cond {
	EQ, NE, CS, CC, MI, PL, VS, VC, HI, LS, GE, LT, GT, LE, AL,
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
};

static const char *const cond_names[] = {
	"eq", "ne", "cs", "cc", "mi", "pl", "vs", "vc",
	"hi", "ls", "ge", "lt", "gt", "le", "al",
};

static const struct inst_encoding inst_encoding[] = {
	/* register */
	{ .mnemonic = "acds", .args = { ARG_LOW_REG, ARG_LOW_REG } },

	/* immediate */
	{ .mnemonic = "adds", .args = { ARG_LOW_REG, ARG_LOW_REG, ARG_IMM3 } },
	{ .mnemonic = "adds", .args = { ARG_LOW_REG, ARG_IMM8 } },
	/* register */
	{ .mnemonic = "adds", .args = { ARG_LOW_REG, ARG_LOW_REG, ARG_LOW_REG } },
	{ .mnemonic = "add", .args = { ARG_NOT_SP_REG, ARG_ANY_REG } },
	/* SP plus immediate */
	{ .mnemonic = "add", .args = { ARG_ANY_REG, ARG_SP, ARG_IMM8_00 } },
	{ .mnemonic = "add", .args = { ARG_SP, ARG_SP, ARG_IMM7_00 } },
	/* SP plus register */
	{ .mnemonic = "add", .args = { ARG_ANY_REG, ARG_SP } },
	{ .mnemonic = "add", .args = { ARG_SP, ARG_ANY_REG } },

	{ .mnemonic = "adr", .args = { ARG_LOW_REG, ARG_IMM8_00_LABEL } },
	{ .mnemonic = "add", .args = { ARG_LOW_REG, ARG_PC, ARG_IMM8_00 } },

	/* register */
	{ .mnemonic = "ands", .args = { ARG_LOW_REG, ARG_LOW_REG } },

	/* immediate */
	{ .mnemonic = "asrs", .args = { ARG_LOW_REG, ARG_LOW_REG, ARG_IMM5 } },
	/* register */
	{ .mnemonic = "asrs", .args = { ARG_LOW_REG, ARG_LOW_REG } },

	{ .mnemonic = "b", .has_cond = true, .args = { ARG_IMM8_0_LABEL } },
	{ .mnemonic = "b", .args = { ARG_IMM11_0_LABEL } },

	/* register */
	{ .mnemonic = "bics", .args = { ARG_LOW_REG, ARG_LOW_REG } },

	{ .mnemonic = "bkpt", .args = { ARG_IMM8 } },

	{ .mnemonic = "bl", .args = { ARG_IMM24_0_LABEL } },

	/* register */
	{ .mnemonic = "blx", .args = { ARG_NOT_PC_REG } },

	{ .mnemonic = "bx", .args = { ARG_NOT_PC_REG } },

	/* register */
	{ .mnemonic = "cmn", .args = { ARG_LOW_REG, ARG_LOW_REG } },
	/* immediate */
	{ .mnemonic = "cmp", .args = { ARG_LOW_REG, ARG_IMM8 } },
	/* register */
	{ .mnemonic = "cmp", .args = { ARG_LOW_REG, ARG_LOW_REG } },
	{ .mnemonic = "cmp", .args = { ARG_HIGH_NOT_SP_REG, ARG_HIGH_NOT_SP_REG } },

	/* register */
	{ .mnemonic = "eors", .args = { ARG_LOW_REG, ARG_LOW_REG } },

	/* load */
	{ .mnemonic = "ldm", .args = { ARG_LOW_REG, ARG_LOW_REG_LS } },
	{ .mnemonic = "ldm", .args = { ARG_LOW_REG, ARG_EXCL, ARG_LOW_REG_LS } },
	/* immediate */
	{ .mnemonic = "ldr", .args = { ARG_LOW_REG, ARG_LOW_REG } },
	{ .mnemonic = "ldr", .args = { ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_IMM5 } },
	{ .mnemonic = "ldr", .args = { ARG_LOW_REG, ARG_SP } },
	{ .mnemonic = "ldr", .args = { ARG_LOW_REG, ARG_BRACE, ARG_SP, ARG_IMM8_00 } },
	/* literal */
	{ .mnemonic = "ldr", .args = { ARG_LOW_REG, ARG_IMM8_00_LABEL } },
	{ .mnemonic = "ldr", .args = { ARG_LOW_REG, ARG_BRACE, ARG_PC, ARG_IMM8_00 } },
	/* register */
	{ .mnemonic = "ldr", .args = { ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_LOW_REG } },

	/* immediate */
	{ .mnemonic = "ldrb", .args = { ARG_LOW_REG, ARG_LOW_REG } },
	{ .mnemonic = "ldrb", .args = { ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_IMM5 } },
	/* register */
	{ .mnemonic = "ldrb", .args = { ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_LOW_REG } },
	/* immediate */
	{ .mnemonic = "ldrh", .args = { ARG_LOW_REG, ARG_LOW_REG } },
	{ .mnemonic = "ldrh", .args = { ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_IMM5 } },
	/* register */
	{ .mnemonic = "ldrh", .args = { ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_LOW_REG } },

	{ .mnemonic = "ldrsb", .args = { ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_LOW_REG } },
	{ .mnemonic = "ldrsh", .args = { ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_LOW_REG } },

	/* immediate */
	{ .mnemonic = "lsls", .args = { ARG_LOW_REG, ARG_LOW_REG, ARG_IMM5 } },
	/* register */
	{ .mnemonic = "lsls", .args = { ARG_LOW_REG, ARG_LOW_REG } },
	/* immediate */
	{ .mnemonic = "lsrs", .args = { ARG_LOW_REG, ARG_LOW_REG, ARG_IMM5 } },
	/* register */
	{ .mnemonic = "lsrs", .args = { ARG_LOW_REG, ARG_LOW_REG } },

	/* immediate */
	{ .mnemonic = "movs", .args = { ARG_LOW_REG, ARG_IMM8 } },
	/* register */
	{ .mnemonic = "movs", .args = { ARG_LOW_REG, ARG_LOW_REG } },
	{ .mnemonic = "movs", .args = { ARG_ANY_REG, ARG_ANY_REG } },

	{ .mnemonic = "muls", .args = { ARG_LOW_REG, ARG_LOW_REG } },

	/* register */
	{ .mnemonic = "mvns", .args = { ARG_LOW_REG, ARG_LOW_REG } },

	{ .mnemonic = "neg", .args = { ARG_LOW_REG, ARG_LOW_REG } },

	{ .mnemonic = "nop" },

	/* register */
	{ .mnemonic = "orrs", .args = { ARG_LOW_REG, ARG_LOW_REG } },

	{ .mnemonic = "pop", .args = { ARG_POP_REG_LS } },
	{ .mnemonic = "push", .args = { ARG_PUSH_REG_LS } },

	/* register */
	{ .mnemonic = "rors", .args = { ARG_LOW_REG, ARG_LOW_REG } },

	/* register */
	{ .mnemonic = "rsbs", .args = { ARG_LOW_REG, ARG_LOW_REG } },
	{ .mnemonic = "sbcs", .args = { ARG_LOW_REG, ARG_LOW_REG } },

	/* store */
	{ .mnemonic = "stm", .args = { ARG_LOW_REG, ARG_LOW_REG_LS } },
	{ .mnemonic = "stm", .args = { ARG_LOW_REG, ARG_EXCL, ARG_LOW_REG_LS } },
	/* immediate */
	{ .mnemonic = "str", .args = { ARG_LOW_REG, ARG_LOW_REG } },
	{ .mnemonic = "str", .args = { ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_IMM5 } },
	{ .mnemonic = "str", .args = { ARG_LOW_REG, ARG_SP } },
	{ .mnemonic = "str", .args = { ARG_LOW_REG, ARG_BRACE, ARG_SP, ARG_IMM8_00 } },
	/* register */
	{ .mnemonic = "str", .args = { ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_LOW_REG } },

	/* immediate */
	{ .mnemonic = "strb", .args = { ARG_LOW_REG, ARG_LOW_REG } },
	{ .mnemonic = "strb", .args = { ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_IMM5 } },
	/* register */
	{ .mnemonic = "strb", .args = { ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_LOW_REG } },
	/* immediate */
	{ .mnemonic = "strh", .args = { ARG_LOW_REG, ARG_LOW_REG } },
	{ .mnemonic = "strh", .args = { ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_IMM5 } },
	/* register */
	{ .mnemonic = "strh", .args = { ARG_LOW_REG, ARG_BRACE, ARG_LOW_REG, ARG_LOW_REG } },

	/* immediate */
	{ .mnemonic = "subs", .args = { ARG_LOW_REG, ARG_LOW_REG, ARG_IMM3 } },
	{ .mnemonic = "subs", .args = { ARG_LOW_REG, ARG_IMM8 } },
	/* register */
	{ .mnemonic = "subs", .args = { ARG_LOW_REG, ARG_LOW_REG, ARG_LOW_REG } },
	/* SP minus immediate */
	{ .mnemonic = "sub", .args = { ARG_SP, ARG_SP, ARG_IMM7_00 } },

	/* register */
	{ .mnemonic = "tst", .args = { ARG_LOW_REG, ARG_LOW_REG } },

	{ .mnemonic = "udf", .args = { ARG_IMM8 } },
};


void armthumb_dump_grammar(void);


#endif
