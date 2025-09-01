/*
 * DELAY.c
 *
 *  Created on: 01/10/2024
 *      Author: André Vaz (48209) e Emilio Mendes (48170)
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdint.h>
#include <stdbool.h>
#include "systick.h"
#include "timer2.h"
#include "delay.h"

static bool delayTimer2State;
static bool delaySystickState;

bool DELAY_GetTimer2State(void){
	return delayTimer2State;
}

bool DELAY_GetSystickState(void){
	return delaySystickState;
}

int DELAY_Init(void)
{
	int state = SYSTICK_Init(SYSTICK_FREQ);
	if( state == 0){
		delaySystickState = true;
		return state;
	} else return state;
}

void DELAY_Milliseconds(uint32_t millis)
{
	uint32_t start = SYSTICK_GetValue();
	while ((SYSTICK_GetValue() - start) < millis) {
		__WFI();
	}
}

uint32_t DELAY_GetElapsedMillis(uint32_t start)
{
	return SYSTICK_GetValue() - start;
}

void DELAY_StopwatchInit( void ){
	if(delayTimer2State) return;
	TIMER2_Init();
	delayTimer2State = true;
}

void DELAY_Stopwatch( uint32_t waitUs ){
	TIMER2_StopWatch(waitUs);
}

