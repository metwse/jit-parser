#ifdef ARMTHUMB

#include "../osal/osal.h"
#include "../plat/armthumb/as.h"


int main(void)
{
	struct armthumb_as as;

	armthumb_as_xinit(&as);

	armthumb_as_xfeed(&as, "adcs");

	struct execmem e = armthumb_as_finalize(&as);

	e.execmem();
}

#endif
