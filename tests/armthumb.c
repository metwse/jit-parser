#ifdef ARMTHUMB

/* #include "../osal/osal.h" */
#include "../plat/armthumb/as.h"


int main(void)
{
	armthumb_static_xinit();

	struct armthumb_as as;

	armthumb_as_xinit(&as);

	armthumb_as_xfeed(&as, "adds r1, r3");
	armthumb_as_xfeed(&as, "push { r2, r3, r4, lr }");
	armthumb_as_xfeed(&as, "pop r1, [r3, #1]");

	/*
	struct execmem e = armthumb_as_finalize(&as);

	e.execmem();
	*/

	armthumb_static_destroy();
}

#endif
