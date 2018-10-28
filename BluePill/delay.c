#include "delay.h"
#include "typedefs.h"
#include "timeout.h"

void DelayMs(u32 ms)
{
	timeout_t t = 0;

	SaveTime(&t);

	while(1)
	{
		if (IsTimeout(&t, ms)) break;
	}
}
