#include "error.h"
#include "typedefs.h"
#include "debug.h"
#include "general.h"

void Error(error_t errorCode)
{
	if (errorCode < ERRORS_COUNT)
	{
		Debug("ERROR %u", errorCode);
	}
	else Error(error_UNKNOWN_ERROR);
}
