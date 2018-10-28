/*
 * ClockValues.h
 *
 *  Created on: Nov 26, 2015
 *      Author: tB
 */

#ifndef ClockValues_H_
#define ClockValues_H_

#include <typedefs.h>


#define TIMESTAMP_START_YEAR 2000 // do wyboru tylko i wy��cznie: 1970/2000/2015

typedef u32 timestamp_t; // value in seconds

typedef enum
{
	Monday = 0,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
	Sunday
}
dayOfWeek_t;

#define DAYS_OF_WEEK_COUNT	7

/*
 * Do struktury humanReadableClockValues_t
 * mo�na tylko dodawa� lub odejmowa� warto�ci.
 * Nie mo�na jej ustawi� od zera.
 * Dzi�ki temu teoretycznie tylko pierwsze
 * wywo�anie funkcji powinno by� czasoch�onne,
 * potem p�tle nie b�d� musia�y si� du�o kr�ci�.
 *
 * Funkcja Init dba tylko o w�a�ciwe ustawienie
 * dnia tygodnia na podstawie roku
 */
typedef struct
{
	timestamp_t timestamp; // Timestamp of these values:
	s8 hours; // 0-23
	s8 minutes; // 0-59
	s8 seconds; // 0-59
	s8 dayOfWeek; // 0-6
	s8 day; // 0-27..30
	s8 month; // 0-11
	s16 year; // 0-65535  (ex. 2015 not 15)
}
clockValues_t;

void ClockValues_Add(clockValues_t * ClockValues, u32 seconds);
void ClockValues_Sub(clockValues_t * ClockValues, u32 seconds);

#endif /* ClockValues_H_ */
