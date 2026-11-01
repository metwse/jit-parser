#ifdef ARMTHUMB

#include "../include/common.h"

#include "../plat/armthumb/as.h"
#include "../plat/armthumb/detail/encoding.h"


static void test_lex(const char *inst,
		     enum inst_opcode op,
		     struct inst_arg_tk *it)
{
	enum inst_opcode parse_op = armthumb_consume_mnemonic(&inst);
	struct inst_arg_tk pit[MAX_PARAM_COUNT + 1];

	assert2(parse_op == op, "opcode mismatch");

	for (size_t i = 0; i < MAX_PARAM_COUNT + 1; i++) {
		pit[i] = armthumb_consume_arg_tk(&inst, i == 0);

		assert2(pit[i].ty == it[i].ty,
			"parsed token type do not match with testsuite");

		switch (pit[i].ty) {
		case INST_ARG_TK_REG:
			assert2(pit[i].seminfo.parsed.reg ==
				it[i].seminfo.parsed.reg,);
			break;
		case INST_ARG_TK_REG_VA_STR:
			break;
		case INST_ARG_TK_REG_VA_NUM:
			break;

		case INST_ARG_TK_REG_LS:
			assert2(pit[i].seminfo.parsed.reg_ls.reg_ls ==
				it[i].seminfo.parsed.reg_ls.reg_ls,);
			break;
		case INST_ARG_TK_REG_LS_VA:
			break;

		case INST_ARG_TK_IMM:
			assert2(pit[i].seminfo.parsed.imm ==
				it[i].seminfo.parsed.imm,);
			break;
		case INST_ARG_TK_IMM_VA:
			break;

		case INST_ARG_TK_COND:
			assert2(pit[i].seminfo.parsed.cond_suffix ==
				it[i].seminfo.parsed.cond_suffix,);
			break;

		case INST_ARG_TK_EXCL:
		case INST_ARG_TK_BRACE:
		case INST_ARG_TK_END:
			break;
		}

		if (pit[i].ty == INST_ARG_TK_END)
			break;
	}
}

int main(void)
{
	armthumb_static_xinit();

	test_lex("adds r1, r3",
		 OP_ADDS,
		 (struct inst_arg_tk []) {
			 { .ty = INST_ARG_TK_REG, .seminfo.parsed.reg = 1 },
			 { .ty = INST_ARG_TK_REG, .seminfo.parsed.reg = 3 },
			 { .ty = INST_ARG_TK_END, },
		 });

	test_lex("push { r2, r3, r4, lr }",
		 OP_PUSH,
		 (struct inst_arg_tk []) {
			 { .ty = INST_ARG_TK_REG_LS, .seminfo.parsed.reg_ls.reg_ls = 540 },
			 { .ty = INST_ARG_TK_END, },
		 });
	test_lex("ldr r1, [r3, #12]",
		 OP_LDR,
		 (struct inst_arg_tk []) {
			 { .ty = INST_ARG_TK_REG, .seminfo.parsed.reg = 1 },
			 { .ty = INST_ARG_TK_BRACE, },
			 { .ty = INST_ARG_TK_REG, .seminfo.parsed.reg = 3 },
			 { .ty = INST_ARG_TK_IMM, .seminfo.parsed.imm = 12 },
			 { .ty = INST_ARG_TK_END, },
		 });
	test_lex("ldm r7!, { r1, r3, r5 }",
		 OP_LDM,
		 (struct inst_arg_tk []) {
			 { .ty = INST_ARG_TK_REG, .seminfo.parsed.reg = 7 },
			 { .ty = INST_ARG_TK_EXCL, },
			 { .ty = INST_ARG_TK_REG_LS, .seminfo.parsed.reg_ls.reg_ls = 42 },
			 { .ty = INST_ARG_TK_END, },
		 });
	test_lex("bne #10",
		 OP_B,
		 (struct inst_arg_tk []) {
			 { .ty = INST_ARG_TK_COND, .seminfo.parsed.cond_suffix = COND_NE },
			 { .ty = INST_ARG_TK_IMM, .seminfo.parsed.imm = 10 },
			 { .ty = INST_ARG_TK_END, },
		 });

	armthumb_static_destroy();
}

#endif
