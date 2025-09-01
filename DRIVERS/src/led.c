/*
 * led.c
 *
 *  Created on: 25/02/2025
 *      Author: André Vaz (48209) e Filipe Cruz (43468)
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdint.h>
#include <stdbool.h>
#include "pin.h"
#include "pinconfig.h"
#include "led.h"

void LED_Init ( LED_ColorValueType ledColorId, bool state, LED_ModelType model )
{
	int led = LED_GetLedByColor( ledColorId );
	PINCFG_Function( led , GPIO);
	PIN_Direction( led , OUTPUT);
	if(state){
		LED_On( ledColorId, model );
	} else{
		LED_Off( ledColorId, model );
	}

}

bool LED_GetState( LED_ColorValueType ledColorId )
{
	int led = LED_GetLedByColor( ledColorId );
	return PIN_Value( led );
}

void LED_On( LED_ColorValueType ledColorId, LED_ModelType model )
{
	int led = LED_GetLedByColor( ledColorId );
	if (model == LED_MODEL_ACTIVE_LOW) PIN_Clear( led );
	else PIN_Set( led );
}

void LED_Off ( LED_ColorValueType ledColorId, LED_ModelType model )
{
	int led = LED_GetLedByColor( ledColorId );
	if (model == LED_MODEL_ACTIVE_LOW) PIN_Set( led );
	else PIN_Clear( led );
}

void LED_Toggle ( LED_ColorValueType ledColorId, LED_ModelType model )
{
	if(LED_GetState( ledColorId )){
		LED_On( ledColorId, model );
	}else{
		LED_Off( ledColorId, model );
	}
}

int LED_GetLedByColor ( LED_ColorValueType ledColorId ){
	int led;

	switch( ledColorId ){
		case LED_INT_RED:
			led = LED_RED_INTERNAL;
			break;
		case LED_INT_GREEN:
			led = LED_GREEN_INTERNAL;
			break;
		case LED_INT_BLUE:
			led = LED_BLUE_INTERNAL;
			break;
		case LED_EXT_RED:
			led = LED_RED_EXTERNAL;
			break;
		case LED_EXT_GREEN:
			led = LED_GREEN_EXTERNAL;
			break;
		case LED_EXT_BLUE:
			led = LED_BLUE_EXTERNAL;
			break;
		case LED_SEM_RED:
			led = LED_RED_SEM;
			break;
		case LED_SEM_YELLOW:
			led = LED_YELLOW_SEM;
			break;
		case LED_SEM_GREEN:
			led = LED_GREEN_SEM;
			break;
		default:
			led = LED_RED_INTERNAL;
	}

	return led;

}
