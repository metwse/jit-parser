#include "encoding.h"

#include <stddef.h>
#include <stdio.h>


void armthumb_dump_grammar(void)
{
	for (size_t inst = 0;
	     inst < INST_COUNT;
	     inst++) {
		for (size_t alt = 0; inst_args[inst][alt][0]; alt++) {
			size_t i = 0;
#define cur_arg inst_args[inst][alt][i]
#define next_arg inst_args[inst][alt][i + 1]

			if (cur_arg == ARG_COND) {
				printf("%s<cond> ", mnemonic_names[inst]);
				i++;
			} else {
				printf("%s ", mnemonic_names[inst]);
			}

			size_t brace_count = 0;
			for (;
			     cur_arg != ARGE;
			     i++) {
				if (next_arg != ARGE &&
				    (cur_arg == ARG_EXCL || (
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
}
