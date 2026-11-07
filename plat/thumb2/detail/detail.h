#ifndef THUMB2_DETAIL_H
#define THUMB2_DETAIL_H

#include "opcodes.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Instruction or sub-assembler. */
struct asm_item {
	enum asm_item_ty {
		ASM_ITEM_INST,
		ASM_ITEM_AS,
	} ty;

	union asm_item_data {
		struct inst inst  /* Instruction */;
		struct mas_thumb2_as *as  /* Sub-assembler. */;
	} item;
};

#define T struct asm_item, asm_item
#include "../../../vendor/libfun/include/stack.h"

#define T size_t, label_idx
#include "../../../vendor/libfun/include/stack.h"

#define T char, struct fstack_label_idx, label_idx_map
#include "../../../vendor/libfun/include/hmap.h"

struct mas_thumb2_as {
	/* Assembly to be emitted as machine code. */
	struct fstack_asm_item asm;
	size_t output_size;

	/* Indexes of instructions that use the label. */
	struct fhmap_label_idx_map label_idx_map;

	/* Upper assembler label scope. */
	struct mas_thumb2_as *up_as;
};


#endif
