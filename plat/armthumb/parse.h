#ifndef ARMTHUMB_PARSE_H
#define ARMTHUMB_PARSE_H

#include "encoding.h"

#include <stdint.h>


/* Parsed instruction. */
struct inst {
	enum inst_mnemonic mnemonic;

	size_t alt_idx;  /* alternative index */

	union inst_params {
		enum {
			LOW_REG_R0, LOW_REG_R1, LOW_REG_R2, LOW_REG_R3,
			LOW_REG_R4, LOW_REG_R5, LOW_REG_R6, LOW_REG_R7,
		} low_reg;

		enum {
			HIGH_REG_R8, HIGH_REG_R9, HIGH_REG_R10, HIGH_REG_R11,
			HIGH_REG_R12, HIGH_REG_R13, HIGH_REG_R14, HIGH_REG_R15,
		} high_reg;

		enum {
			ANY_REG_R0, ANY_REG_R1, ANY_REG_R2, ANY_REG_R3,
			ANY_REG_R4, ANY_REG_R5, ANY_REG_R6, ANY_REG_R7,
			ANY_REG_R8, ANY_REG_R9, ANY_REG_R10, ANY_REG_R11,
			ANY_REG_R12, ANY_REG_R13, ANY_REG_R14, ANY_REG_R15,
		} any_reg;

		uint32_t imm;
	} params[MAX_ARG_COUNT - 1];
};


#endif
