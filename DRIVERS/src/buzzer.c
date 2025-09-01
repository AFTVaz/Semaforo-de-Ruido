/*
 * buzzer.c
 *
 *  Created on: 21/03/2025
 *      Author: André Vaz (48209) e Filipe Cruz (43468)
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdint.h>
#include <stdbool.h>
#include "pin.h"
#include "pinconfig.h"
#include "buzzer.h"
#include "delay.h"

void BUZZER_Init( void ){
	DELAY_StopwatchInit();
	PINCFG_Function( BUZZER_PIN , GPIO);
	PIN_Direction(BUZZER_PIN , OUTPUT);
}

void BUZZER_On( void ){
	PIN_Set(BUZZER_PIN);
}

void BUZZER_Off( void ){
	PIN_Clear(BUZZER_PIN);
}

void BUZZER_Alarm( uint32_t time ){
	BUZZER_On();
	DELAY_Stopwatch( time );
	BUZZER_Off();
}
