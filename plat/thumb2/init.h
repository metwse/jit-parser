/* Static initializers for the Thumb-2 assembler. */

#ifndef THUMB2_INIT_H
#define THUMB2_INIT_H

#include "../../include/detail.h"


/* Initialize static variables for the ARM thumb assembler. */
_masi void thumb2_static_xinit(void);

/* Free the resources owned by static variables of the ARM thumb assembler. */
_masi void thumb2_static_destroy(void);


#endif
