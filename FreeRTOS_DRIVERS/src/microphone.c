/*
 * microphone.c
 *
 *  Created on: 04/06/2025
 *      Author: André Vaz (48209) e Filipe Cruz (43468)
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "max9814.h"
#include "microphone.h"

static SemaphoreHandle_t xMicMutex = NULL;

void MICROPHONE_Init( MAX9814_GainValueType gain ){
	if (xMicMutex == NULL) {
		xMicMutex = xSemaphoreCreateMutex();
		if (xMicMutex != NULL) {
			MAX9814_Init(gain);
		}
	}
}

float MICROPHONE_Read( void ){
	float value = 0.0f;

	if (xMicMutex != NULL) {
		if (xSemaphoreTake(xMicMutex, portMAX_DELAY) == pdTRUE) {
			value = MAX9814_Read();
			xSemaphoreGive(xMicMutex);
		}
	}

	return value;
}

void MICROPHONE_SetGain( MAX9814_GainValueType gain ){
	if( xMicMutex != NULL ){
		if( xSemaphoreTake(xMicMutex, portMAX_DELAY) == pdTRUE ){
			MAX9814_SetGain( gain );
			xSemaphoreGive( xMicMutex );
		}
	}
}
