#ifdef ARMTHUMB

#include "../plat/armthumb/as.h"


int main(void)
{
	armthumb_static_xinit();

	struct armthumb_as as;
	armthumb_as_xinit(&as);

	armthumb_as_xfeed(&as, "push %l", 0x1 | 0x2);
	armthumb_as_xfeed(&as, "ldm %r, { r2, r3 }", 1);
	armthumb_as_xfeed(&as, "adds r1, %r, %R", 2, "r5");
	armthumb_as_xfeed(&as, "adds r1, %R, %r", "r2", 0);
	armthumb_as_xfeed(&as, "str %r, [%r, %i]", 2, 4, 6);

	struct execmem e = armthumb_as_finalize(&as);

	execmem_free(&e);

	armthumb_static_destroy();
}

#endif
