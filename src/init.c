#include "../include/mas.h"

#ifdef PLAT_thumb2
#include "../plat/thumb2/init.h"
#endif


void mas_xinit(void)
{
#ifdef PLAT_thumb2
	thumb2_static_xinit();
#endif
}

void mas_destroy(void)
{
#ifdef PLAT_thumb2
	thumb2_static_destroy();
#endif
}
