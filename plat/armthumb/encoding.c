#include "encoding.h"

#include <stddef.h>
#include <stdio.h>


void armthumb_dump_grammar(void)
{
	for (size_t i = 0;
	     i < sizeof(inst_encoding) / sizeof(struct inst_encoding);
	     i++) {
		struct inst_encoding inst = inst_encoding[i];

		if (inst.has_cond)
			printf("%s<cond> ", inst.mnemonic);
		else
			printf("%s ", inst.mnemonic);

		size_t brace_count = 0;
		for (size_t j = 0;
		     inst.args[j] != ARGE;
		     j++) {
#define cur_arg inst.args[j]
#define next_arg inst.args[j + 1]
			if (next_arg != ARGE && (cur_arg == ARG_EXCL || (
				(is_reg(cur_arg) || is_imm(cur_arg)) &&
				(next_arg != ARG_EXCL))
			    )) {
				printf("%s, ", arg_names[cur_arg]);
			} else if (cur_arg == ARG_BRACE) {
				printf("[");
				brace_count++;
			} else if (next_arg == ARGE || next_arg == ARG_EXCL) {
				printf("%s", arg_names[cur_arg]);
			} else {
				printf("%s ", arg_names[cur_arg]);
			}
		}

		while (brace_count--)
			printf("]");

		printf("\n");
	}
}
