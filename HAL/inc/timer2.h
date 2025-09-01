/**
* @file timer2.h
* @brief Contains the timer2 API.
* @version 1.0
* @date 8 Out 2024
* @author André Vaz (48209) e Emilio Mendes (48170)
*
*
**********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
*********************************************************************/

/** @defgroup HAL HAL
* @brief This package provides drivers layer functions.
* @{
*/

/** @defgroup TIMER2 TIMER2
* @brief Provides the core capabilities for timer2 functions.
* @{
*/

#ifndef _TIMER2_H_
#define _TIMER2_H_


#define TIMER2_PCONP_ENABLE 22
#define TIMER2_PCLKSEL1     12
#define ENABLE_BIT_POS		0
#define RESET_BIT_POS		1
#define MR0I_BIT_POS		0
#define MR0R_BIT_POS		1
#define MR0S_BIT_POS		2

/**
 * @brief Initializes Timer2 with a 1 µs resolution.
 *
 * 		  Powers up Timer2, selects the peripheral clock (PCLK) for Timer2,
 * 		  resets the timer, and sets the prescaler to generate a tick every 1 microsecond.
 *
 * @note This configuration assumes PCLK = SystemCoreClock / 4.
 */
void TIMER2_Init(void);

/**
 * @brief Performs a blocking delay using Timer2.
 *
 * 		  Starts Timer2 in stopwatch mode to wait for a specified time (in microseconds).
 * 		  The timer generates an interrupt on match, resets, and stops automatically.
 * 		  The function blocks until the match event occurs.
 *
 * @param wait Time to wait, in microseconds.
 *
 * @note This function is blocking and uses Match Register 0 (MR0).
 */
void TIMER2_StopWatch(uint32_t wait);


#endif /* _TIMER2_H_ */

/**
* @}
*/
/**
* @}
*/
