/*
 * timer2.c
 *
 *  Created on: 08/10/2024
 *      Author: André Vaz (48209) e Emilio Mendes (48170)
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdint.h>
#include <stdio.h>
#include "timer2.h"

#define PCLK_TIMER2 SystemCoreClock/4

void TIMER2_Init(void){
	LPC_SC->PCONP |= (1 << TIMER2_PCONP_ENABLE);	//Enable Timer2 power
	LPC_SC->PCLKSEL1 &= ~(3 << TIMER2_PCLKSEL1); 	//seleção do clock para o timer2
	LPC_TIM2->TCR = 1 << RESET_BIT_POS;				//deixar RESET ativo
	LPC_TIM2->PR = (PCLK_TIMER2 / 1000000) - 1;		//definir o prescale register
}

void TIMER2_StopWatch(uint32_t wait){
	LPC_TIM2->TCR = 1 << RESET_BIT_POS;
	LPC_TIM2->MCR = (1 << MR0I_BIT_POS) | (1 << MR0R_BIT_POS) | (1 << MR0S_BIT_POS);	//On Match, Interrupt, Reset and Stop
	LPC_TIM2->MR0 = wait;
	LPC_TIM2->TCR = 1 << ENABLE_BIT_POS;		//Ativar o Contador

	LPC_TIM2->IR = 1;
	while(LPC_TIM2->IR == 0x0);
}


