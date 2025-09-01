/*
 * buzzer_rtos.c
 *
 *  Created on: 21/03/2025
 *      Author: André Vaz (48209) e Filipe Cruz (43468)
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "buzzer.h"
#include "buzzer_rtos.h"

#define BUZZER_DEFAULT_ALARM_DURATION 	pdMS_TO_TICKS(1000)

static TaskHandle_t xBuzzerTaskHandle = NULL;
static TimerHandle_t buzzerTimer = NULL;

static void buzzerTimerCallback( TimerHandle_t xTimer ){
	BUZZER_Off();
}

static void BUZZER_InitTimer( void ){
	if (buzzerTimer == NULL){
		buzzerTimer = xTimerCreate(
				"BuzzerTimer",
				BUZZER_DEFAULT_ALARM_DURATION,
				pdFALSE,
				NULL,
				buzzerTimerCallback
		);
	}
}

static void BUZZER_RTOS_Alarm_Start( uint32_t duration_ms ){
	BUZZER_On();

	if (buzzerTimer == NULL){
		BUZZER_InitTimer();
	}

	xTimerChangePeriod(buzzerTimer, pdMS_TO_TICKS(duration_ms), 0);
	xTimerStart(buzzerTimer, 0);
}

static void BUZZER_RTOS_Alarm_Stop( void ){
	if ((buzzerTimer != NULL) && (xTimerIsTimerActive(buzzerTimer) == pdTRUE)){
		xTimerStop(buzzerTimer, 0);
	}
}

void BUZZER_RTOS_Init( void ){
	xTaskCreate( BUZZER_RTOS_Task, "Buzzer_Task",
			configMINIMAL_STACK_SIZE, NULL, 1, &xBuzzerTaskHandle );
}

void BUZZER_RTOS_Task( void *pvParameters ){
	BUZZER_Init();
	BUZZER_InitTimer();

	uint32_t command;

	while( true ){
		if (xTaskNotifyWait(0, 0xFFFFFFFF, &command, portMAX_DELAY) == pdPASS) {
			BUZZER_Command cmd = (BUZZER_Command)(command & 0xFF);
			uint32_t duration = (command >> 8) & 0xFFFFFF;

			switch (cmd) {
				case BUZZER_CMD_ON:
					BUZZER_RTOS_Alarm_Stop();
					BUZZER_On();
					break;

				case BUZZER_CMD_OFF:
					BUZZER_RTOS_Alarm_Stop();
					BUZZER_Off();
					break;

				case BUZZER_CMD_ALARM:
					BUZZER_RTOS_Alarm_Start(duration);
					break;

				default:
					break;
			}
		}
	}
}

bool BUZZER_RTOS_Ready( void ) {
	return xBuzzerTaskHandle != NULL;
}

static void BUZZER_RTOS_SendNotify( uint32_t command ) {
	if (xBuzzerTaskHandle != NULL) {
		xTaskNotify(xBuzzerTaskHandle, command, eSetValueWithOverwrite);
	}
}


void BUZZER_RTOS_On( void ){
	BUZZER_RTOS_SendNotify( BUZZER_CMD_ON );
}

void BUZZER_RTOS_Off( void ){
	BUZZER_RTOS_SendNotify( BUZZER_CMD_OFF );
}

void BUZZER_RTOS_Alarm( uint32_t duration_ms ){
	uint32_t cmd = ((duration_ms & 0xFFFFFF) << 8) | BUZZER_CMD_ALARM;
	BUZZER_RTOS_SendNotify(cmd);
}

