/*
 * systick.c
 *
 *  Created on: 01/10/2024
 *      Author: André Vaz (48209) e Emilio Mendes (48170)
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdint.h>
#include "systick.h"
#include "FreeRTOS.h"

static volatile uint32_t __tick;

void SysTick_Handler(void)
{
	__tick++;
}

int SYSTICK_Init(uint32_t value)
{
#ifdef BAREMETAL
	SystemCoreClockUpdate();
	if (SysTick_Config(value) == 1) return -1;
#endif
	return 0;
}

uint32_t SYSTICK_GetValue(void)
{
#ifdef BAREMETAL
	return __tick;
#else
	return xTaskGetTickCount();
#endif
}
