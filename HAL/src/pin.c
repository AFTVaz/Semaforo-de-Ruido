/*
 * pin.c
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

#define LPC_GPIO_SIZE 			0x20
#define LPC_GPIO_NUM_PINS 		32


void PIN_Direction(PIN_ValueType pinId, PIN_DIRType dir)
{
	uint32_t port = pinId / LPC_GPIO_NUM_PINS;
	uint32_t pin = pinId % LPC_GPIO_NUM_PINS;
	LPC_GPIO_TypeDef *gpio = (LPC_GPIO_TypeDef *)(LPC_GPIO_BASE + (port * LPC_GPIO_SIZE));

	if( dir == OUTPUT ){
		gpio->FIODIR |= (0x1UL << pin);
	}
	else{
		gpio->FIODIR &= ~(0x1UL << pin);
	}
}

void PIN_Set(PIN_ValueType pinId)
{
	uint32_t port = pinId /LPC_GPIO_NUM_PINS;
	uint32_t pin = pinId % LPC_GPIO_NUM_PINS;
	LPC_GPIO_TypeDef *gpio=(LPC_GPIO_TypeDef *)(LPC_GPIO_BASE+(port * LPC_GPIO_SIZE));

	gpio->FIOSET = ( 1 << pin );
}

void PIN_Clear(PIN_ValueType pinId)
{
	uint32_t port = pinId /LPC_GPIO_NUM_PINS;
	uint32_t pin = pinId % LPC_GPIO_NUM_PINS;
	LPC_GPIO_TypeDef *gpio=(LPC_GPIO_TypeDef *)(LPC_GPIO_BASE+(port * LPC_GPIO_SIZE));

	gpio->FIOCLR = ( 1 << pin );
}

uint32_t PIN_Value( PIN_ValueType pinId )
{
	uint32_t port = pinId /LPC_GPIO_NUM_PINS;
	uint32_t pin = pinId % LPC_GPIO_NUM_PINS;
	LPC_GPIO_TypeDef *gpio=(LPC_GPIO_TypeDef *)(LPC_GPIO_BASE+(port * LPC_GPIO_SIZE));

	return (gpio->FIOPIN & (1 << pin));
}


void PIN_PortWrite( PIN_PortType portId, uint32_t value, uint32_t mask )
{
	LPC_GPIO_TypeDef *gpio=(LPC_GPIO_TypeDef *)(LPC_GPIO_BASE+(portId * LPC_GPIO_SIZE));

	gpio->FIOPIN = ((gpio->FIOSET & ~mask) | (value & mask));
}
