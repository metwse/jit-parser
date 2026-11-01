#include "as.h"
#include "detail/opcodes.h"
#include "detail/binary.h"

#include "../../osal/osal.h"

#include <stdbool.h>
#include <stdint.h>


/* emit to an offset */
#define emit(bits, n, o) do { \
		bytes |= ((bits) & ((1 << (n)) - 1)) << o; \
	} while (0)
/* emit bytes incrementing saved offset */
#define emit2(bits, n) do { \
		emit(bits, n, write_offset); \
		write_offset += n; \
	} while (0)
/* emit for the rest */
#define emit3(bits) do { \
		emit2(bits, (is_32_bit ? 32 : 16) - write_offset); \
	} while (0)


void armthumb_emit(struct armthumb_as *as, struct inst inst)
{
	uint32_t bytes = 0;
	bool is_32_bit = false;
	uint32_t write_offset = 0;

	size_t alt_idx = inst.alt_idx;
	union inst_arg_data *args = inst.args;


	switch (inst.opcode) {
	case OP_ADCS:
		switch (alt_idx) {
		case 0:
			emit2(args[0].reg, 3);
			emit2(args[1].reg, 3);
			emit3(b(0100000101));
			break;
		}
		break;

	case OP_ADDS:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_ADD:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_ADR:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_ANDS:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_ASRS:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_B:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_BICS:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_BKPT:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_BL:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_BLX:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_BX:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_CMN:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_CMP:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_EORS:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_LDM:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_LDR:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_LDRB:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_LDRH:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_LDRSB:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_LDRSH:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_LSLS:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_LSRS:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_MOVS:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_MULS:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_MVNS:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_NEG:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_NOP:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_ORRS:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_POP:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_PUSH:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_RORS:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_RSBS:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_SBCS:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_STM:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_STR:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_STRB:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_STRH:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_SUBS:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_SUB:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_TST:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;

	case OP_UDF:
		switch (alt_idx) {
		case 0:
			break;
		}
		break;
	}


	if (as->e.size + 4 == as->i)
		execmem_xrealloc(&as->e, as->e.size * 2);

	uint16_t *addr = &((uint16_t *) as->e.mem)[is_32_bit ? as->i + 1 : as->i];

	if (!is_32_bit) {
		*addr = (uint16_t) bytes;
		as->i += 1;
	} else {
		*(uint32_t *) addr = bytes;
		as->i += 2;
	}
}
