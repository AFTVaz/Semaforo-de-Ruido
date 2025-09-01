/*
 * clock.c
 *
 *  Created on: 17/03/2025
 *      Author: André Vaz (48209) e Filipe Cruz (43468)
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "rtc.h"
#include "clock.h"


static SemaphoreHandle_t xClockMutex = NULL;

void CLOCK_Init( void ){
	if (xClockMutex == NULL) {
		xClockMutex = xSemaphoreCreateMutex();
		if (xClockMutex != NULL) {
			RTC_Init(0);
		}
	}
}

void CLOCK_GetTimeDate ( struct tm *dateTime ){
	if( xClockMutex != NULL ) {
		if(xSemaphoreTake(xClockMutex , portMAX_DELAY) == pdTRUE) {
			RTC_GetTimeDate ( dateTime );
			xSemaphoreGive(xClockMutex);
		}
	}
}

void CLOCK_SetTimeDate( struct tm *dateTime ){
	if( xClockMutex != NULL ) {
		if(xSemaphoreTake(xClockMutex , portMAX_DELAY) == pdTRUE) {
			RTC_SetTimeDate ( dateTime );
			xSemaphoreGive(xClockMutex);
		}
	}
}

void CLOCK_SetSeconds ( time_t seconds ){
	if( xClockMutex != NULL ) {
		if(xSemaphoreTake(xClockMutex , portMAX_DELAY) == pdTRUE) {
			RTC_SetSeconds ( seconds );
			xSemaphoreGive(xClockMutex);
		}
	}
}

time_t CLOCK_GetSeconds ( void ){
	time_t value = 0;

	if (xClockMutex != NULL) {
		if (xSemaphoreTake(xClockMutex , portMAX_DELAY) == pdTRUE) {
			value = RTC_GetSeconds();
			xSemaphoreGive(xClockMutex);
		}
	}

	return value;
}
