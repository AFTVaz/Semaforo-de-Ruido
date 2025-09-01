/*
 * adc.c
 *
 *  Created on: 03/04/2025
 *      Author: André Vaz (48209) e Filipe Cruz (43468)
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdint.h>
#include <stdio.h>
#include "adc.h"
#include "pin.h"
#include "pinconfig.h"

void ADC_Init( uint8_t channel ) {
	if(channel > 7) return;

	LPC_SC->PCONP |= (1 << 12);
	LPC_SC->PCLKSEL0 &= ~(3 << 24);

	switch(channel) {
        case 0:
        	PINCFG_Function(ADC_AD00_PIN, AD0_F1); // P0.23 como AD0.0
        	PINCFG_Mode(ADC_AD00_PIN, NONE);
        	break;
        case 1:
        	PINCFG_Function(ADC_AD01_PIN, AD0_F1); // P0.24 como AD0.1
        	PINCFG_Mode(ADC_AD01_PIN, NONE);
        	break;
        case 2:
        	PINCFG_Function(ADC_AD02_PIN, AD0_F1); // P0.25 como AD0.2
        	PINCFG_Mode(ADC_AD02_PIN, NONE);
        	break;
        case 3:
        	PINCFG_Function(ADC_AD03_PIN, AD0_F1); // P0.26 como AD0.3
        	PINCFG_Mode(ADC_AD03_PIN, NONE);
        	break;
        case 4:
        	PINCFG_Function(ADC_AD04_PIN, AD0_F3); // P1.30 como AD0.4
        	PINCFG_Mode(ADC_AD04_PIN, NONE);
        	break;
        case 5:
        	PINCFG_Function(ADC_AD05_PIN, AD0_F3); // P1.31 como AD0.5
        	PINCFG_Mode(ADC_AD05_PIN, NONE);
        	break;
        case 6:
        	PINCFG_Function(ADC_AD06_PIN, AD0_F2); // P0.2 como AD0.6
        	PINCFG_Mode(ADC_AD06_PIN, NONE);
        	break;
        case 7:
        	PINCFG_Function(ADC_AD07_PIN, AD0_F2); // P0.3 como AD0.7
        	PINCFG_Mode(ADC_AD07_PIN, NONE);
        	break;
    }

    LPC_ADC->ADCR = ( 1 << channel ) | ( 4 << ADCR_CLKDIV ) | ( 1 << ADCR_PDN );
}

uint16_t ADC_Read(uint8_t channel) {
	LPC_ADC->ADCR &= ~( ADCR_CLEAN_SEL );
	LPC_ADC->ADCR |= ( 1 << channel );
	LPC_ADC->ADCR |= ( 1 << ADCR_START );

	while (!(LPC_ADC->ADGDR & (1 << 31)));

	LPC_ADC->ADCR &= ~(7 << ADCR_START);

	return (LPC_ADC->ADGDR >> 4) & 0xFFF;
}
