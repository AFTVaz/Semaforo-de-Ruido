/*
 * pinconfig.c
 *
 *  Created on: 01/10/2024
 *      Author: André Vaz (48209) e Emilio Mendes (48170)
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdint.h>
#include "pin.h"
#include "pinconfig.h"

void PINCFG_Function(PIN_ValueType pinId, PINCFG_FunctionType func){
	uint32_t port = pinId / LPC_GPIO_NUM_PINS;
	uint32_t pin = pinId % LPC_GPIO_NUM_PINS;
	uint32_t pinsel = 2 * port;
	uint32_t *regist = (uint32_t *) & LPC_PINCON->PINSEL0;
	if (pin >= 16) {
		pin -= 16;
		pinsel++;
	}
	*(uint32_t *) (regist + pinsel) &= ~(0x03UL << (pin * 2));
	*(uint32_t *) (regist + pinsel) |= ((uint32_t) func) << (pin * 2);
}

void PINCFG_Mode(PIN_ValueType pinId, PINCFG_ResistorModeType mode){
	uint32_t port = pinId / LPC_GPIO_NUM_PINS;
	uint32_t pin = pinId % LPC_GPIO_NUM_PINS;
	uint32_t pinsel = 2 * port;
	uint32_t *regist = (uint32_t *) & LPC_PINCON->PINMODE0;
	if (pin >= 16) {
		pin -= 16;
		pinsel++;
	}
	*(uint32_t *) (regist + pinsel) &= ~(0x03UL << (pin * 2));
	*(uint32_t *) (regist + pinsel) |= ((uint32_t) mode) << (pin * 2);
}

void PINCFG_OpenDrainMode(PIN_ValueType pinId, PINCFG_OpenDrainModeType mode){
	uint32_t port = pinId / LPC_GPIO_NUM_PINS;
	uint32_t pin = pinId % LPC_GPIO_NUM_PINS;
	uint32_t pinsel = 2 * port;
	uint32_t *regist = (uint32_t *) & LPC_PINCON->PINMODE_OD0;
	if (pin >= 16) {
		pin -= 16;
		pinsel++;
	}
	*(uint32_t *) (regist + pinsel) &= ~(0x03UL << (pin * 2));
	*(uint32_t *) (regist + pinsel) |= ((uint32_t) mode) << (pin * 2);
}

