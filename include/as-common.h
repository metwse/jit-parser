/* Common assembler options & methods. */

#ifndef MAS_AS_COMMON_H
#define MAS_AS_COMMON_H


/* Assembler allocation hints. */
enum mas_as_hint {
	/* The assembler will be small in size (e.g. part of an if/else branch),
	 * do not pre-allocate large maps structs for it. Assemblers of this
	 * kind may allocated from object pool. */
	MAS_AS_SMALL = 1,
};


#endif
