#ifdef ARMTHUMB

#include "../plat/armthumb/as.h"


int main(void)
{
	armthumb_static_xinit();

	struct armthumb_as as;
	armthumb_as_xinit(&as);

	armthumb_as_xfeed(&as, "adcs r1, r2");

	armthumb_as_xfeed(&as, "adds r1, r2, %i", 7);
	armthumb_as_xfeed(&as, "adds r1, #255");
	armthumb_as_xfeed(&as, "adds r3, r2, r1");

	armthumb_as_xfeed(&as, "add lr, r15");
	armthumb_as_xfeed(&as, "add lr, r5");
	armthumb_as_xfeed(&as, "add r7, sp, #1016");
	armthumb_as_xfeed(&as, "add sp, #504");
	armthumb_as_xfeed(&as, "add pc, sp");
	armthumb_as_xfeed(&as, "add r1, sp");
	armthumb_as_xfeed(&as, "add sp, lr");
	armthumb_as_xfeed(&as, "add sp, r2");
	armthumb_as_xfeed(&as, "add r2, %R", "pc");

	armthumb_as_xfeed(&as, "ands r2, r4");

	armthumb_as_xfeed(&as, "asrs r2, r4, #31");
	armthumb_as_xfeed(&as, "asrs r4, r3");

	armthumb_as_xfeed(&as, "bne #4");
	armthumb_as_xfeed(&as, "b #4");

	armthumb_as_xfeed(&as, "bics r3, r7");
	armthumb_as_xfeed(&as, "bkpt #170");

	armthumb_as_xfeed(&as, "bl #16777216");

	armthumb_as_xfeed(&as, "blx %r", 11);
	armthumb_as_xfeed(&as, "bx %r", 10);
	armthumb_as_xfeed(&as, "cmn r3, r5");

	armthumb_as_xfeed(&as, "cmp r3, %i", 255);
	armthumb_as_xfeed(&as, "cmp r2, r1");
	armthumb_as_xfeed(&as, "cmp pc, sp");
	armthumb_as_xfeed(&as, "cmp r1, sp");
	armthumb_as_xfeed(&as, "cmp pc, r1");

	armthumb_as_xfeed(&as, "eors r1, r4");

	armthumb_as_xfeed(&as, "ldm r1!, { r4 }");
	armthumb_as_xfeed(&as, "ldm r1, { r2, r4 }");
	armthumb_as_xfeed(&as, "ldm r2!, { r4 }");
	armthumb_as_xfeed(&as, "ldm r3, { r4, r5, r6 }");

	armthumb_as_xfeed(&as, "ldr r1, r2");
	armthumb_as_xfeed(&as, "ldr r1, [r2, #124]");
	armthumb_as_xfeed(&as, "ldr r3, sp");
	armthumb_as_xfeed(&as, "ldr r4, [sp, #1016]");
	armthumb_as_xfeed(&as, "ldr r5, #1012");
	armthumb_as_xfeed(&as, "ldr r6, [pc, #1008]");
	armthumb_as_xfeed(&as, "ldr r7, [r1, r2]");

	struct execmem e = armthumb_as_finalize(&as);
	e.execmem();
	execmem_free(&e);

	armthumb_static_destroy();
}

#endif
