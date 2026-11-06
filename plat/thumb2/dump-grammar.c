#include "detail.h"

#include <stddef.h>
#include <stdio.h>


void thumb2_dump_grammar(void)
{
	for (size_t inst = 0;
	     inst < INST_COUNT;
	     inst++) {
		for (size_t alt = 0; inst_params[inst][alt][0]; alt++) {
			size_t i = 0;
#define cur_param inst_params[inst][alt][i]
#define next_param inst_params[inst][alt][i + 1]

			if (cur_param == PARAM_COND) {
				printf("%s<cond> ", mnemonics[inst]);
				i++;
			} else {
				printf("%s ", mnemonics[inst]);
			}

			size_t brace_count = 0;
			for (;
			     cur_param != PARAME;
			     i++) {
				if (next_param != PARAME &&
				    (cur_param == PARAM_EXCL || (
					(is_reg(cur_param) || is_imm(cur_param)) &&
					(next_param != PARAM_EXCL))
				    )) {
					printf("%s, ", param_names[cur_param]);
				} else if (cur_param == PARAM_BRACE) {
					printf("[");
					brace_count++;
				} else if (next_param == PARAME || next_param == PARAM_EXCL) {
					printf("%s", param_names[cur_param]);
				} else {
					printf("%s ", param_names[cur_param]);
				}
			}

			while (brace_count--)
				printf("]");

			printf("\n");
		}
	}
}
