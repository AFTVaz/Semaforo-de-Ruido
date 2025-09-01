/**
* @file delay.h
* @brief Contains the delay API.
* @version 1.0
* @date 1 Out 2024
* @author André Vaz (48209) e Emilio Mendes (48170)
*
*
**********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
*********************************************************************/

/** @defgroup DRIVERS DRIVERS
* @brief This package provides drivers layer functions.
* @{
*/

/** @defgroup DELAY DELAY
* @brief Provides the core capabilities for time delaying functions.
* @{
*/

#ifndef _DELAY_H_
#define _DELAY_H_

#include <stdbool.h>

/**
 * @def SYSTICK_FREQ
 * @brief Macro that defines the SysTick frequency as 1 ms.
 *
 * SYSTICK_FREQ = (SystemCoreClock / 1000)
 */
#define SYSTICK_FREQ (SystemCoreClock/1000)


/**
 * @brief Returns whether Timer2 has been initialized.
 * @return true if Timer2 is initialized, false otherwise.
 */
bool DELAY_GetTimer2State(void);

/**
 * @brief Returns whether SysTick has been initialized.
 * @return true if SysTick is initialized, false otherwise.
 */
bool DELAY_GetSystickState(void);

/**
 * @brief Initializes the SysTick delay system.
 * @return 0 if initialization was successful, -1 otherwise.
 */
int DELAY_Init(void);

/**
 * @brief Delays the execution for a given number of milliseconds.
 * @param millis Number of milliseconds to wait.
 */
void DELAY_Milliseconds(uint32_t millis);

/**
 * @brief Returns the number of milliseconds that have elapsed since a start point.
 * @param start The starting value returned by SYSTICK_GetValue().
 * @return Elapsed milliseconds.
 */
uint32_t DELAY_GetElapsedMillis(uint32_t start);

/**
 * @brief Initializes the Timer2 stopwatch for microsecond precision delays.
 */
void DELAY_StopwatchInit( void ) ;

/**
 * @brief Delays the execution for a given number of microseconds.
 * @param waitUs Number of microseconds to wait.
 */
void DELAY_Stopwatch( uint32_t waitUs );

#endif /* _DELAY_H_ */

/**
* @}
*/
/**
* @}
*/
