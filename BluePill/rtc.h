#ifndef RTC_H_
#define RTC_H_

#include <typedefs.h>
#include <clockvalues.h>

void RTC_Init(void);

void Clock_Set(timestamp_t timestamp);
timestamp_t Clock_Get(void);
void Clock_AddSeconds(u32 secondsToAdd);
void Clock_SubtractSeconds(u32 secondsToSubtract);

u8 Clock_GetHours(void);
u8 Clock_GetMinutes(void);
u8 Clock_GetSeconds(void);
u8 Clock_GetDayOfWeek(void);
u8 Clock_GetDay(void);
u8 Clock_GetMonth(void);
u16 Clock_GetYear(void);

#endif /* RTC_H_ */
