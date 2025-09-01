/*
 * rtc.c
 *
 *  Created on: 05/11/2024
 *      Author: André Vaz (48209) e Emilio Mendes (48170)
 */


#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "rtc.h"

int compareByTime(const struct tm *a, const struct tm *b){
	int res = a->tm_sec - b->tm_sec;
	if(res != 0) return res;
	res = a->tm_min - b->tm_min;
	if(res != 0) return res;
	res = a->tm_hour - b->tm_hour;
	return res;
}

void RTC_Init ( time_t seconds ){
	LPC_SC->PCONP |= (1 << RTC_PCONP_ENABLE);

	LPC_RTC->CCR = ~( 1 << CLKEN_BIT_POS);		//Clock disable

	RTC_SetSeconds(seconds);

	LPC_RTC->CCR = (1 << CLKEN_BIT_POS);		//Clock enable
}

void RTC_GetTimeDate( struct tm *dateTime ){
	uint32_t CTIME0 = LPC_RTC->CTIME0;
	uint32_t CTIME1 = LPC_RTC->CTIME1;
	uint32_t CTIME2 = LPC_RTC->CTIME2;

	dateTime->tm_sec = (CTIME_SEC_MASK & CTIME0);
	dateTime->tm_min = (CTIME_MIN_MASK & CTIME0) >> 8;
	dateTime->tm_hour = (CTIME_HOUR_MASK & CTIME0) >> 16;
	dateTime->tm_wday = (CTIME_DOW_MASK & CTIME0) >> 24;
	dateTime->tm_mday = (CTIME_DOM_MASK & CTIME1);
	dateTime->tm_mon = ((CTIME_MONTH_MASK & CTIME1) >> 8) - 1;
	dateTime->tm_year = ((CTIME_YEAR_MASK & CTIME1) >> 16) - 1900;
	dateTime->tm_yday = (CTIME_DOY_MASK & CTIME2) - 1;
	dateTime->tm_isdst = 0;		//Daylight Saving is not in effect
}

void RTC_SetTimeDate( struct tm *dateTime ){
	struct tm testTime;
	RTC_GetTimeDate( &testTime );

	while(compareByTime(&testTime, dateTime)){

		LPC_RTC->CCR = ~( 1 << CLKEN_BIT_POS);		//Clock disable

		LPC_RTC->SEC = dateTime->tm_sec;
		LPC_RTC->MIN = dateTime->tm_min;
		LPC_RTC->HOUR = dateTime->tm_hour;
		LPC_RTC->DOM = dateTime->tm_mday;
		LPC_RTC->DOW = dateTime->tm_wday;
		LPC_RTC->DOY = dateTime->tm_yday + 1;
		LPC_RTC->MONTH = dateTime->tm_mon + 1;
		LPC_RTC->YEAR = dateTime->tm_year + 1900;

		LPC_RTC->CCR = (1 << CLKEN_BIT_POS);		//Clock enable

		RTC_GetTimeDate( &testTime );
	}
}

void RTC_SetSeconds( time_t seconds ){

	struct tm *time = localtime(&seconds);
	RTC_SetTimeDate(time);

}

time_t RTC_GetSeconds( void ){

	struct tm dtime;
	RTC_GetTimeDate(&dtime);
	return mktime(&dtime);

}

