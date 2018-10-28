#include "timeout.h"
#include "typedefs.h"
#include "systick.h"

void SaveTime(timeout_t * timeKeeper)
{
	*timeKeeper = sysTickValue;
}

bool IsTimeout(timeout_t * timeKeeper, timeout_t timeout)
{
	if (sysTickValue - *timeKeeper > timeout) return true;

	return false;
}

bool Tick(timeout_t * timeKeeper, timeout_t tickInterval)
{
	if (IsTimeout(timeKeeper, tickInterval))
	{
		SaveTime(timeKeeper);

		return true;
	}

	return false;
}
