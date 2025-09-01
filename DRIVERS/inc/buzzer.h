/**
* @file buzzer.h
* @brief Contains the Buzzer API.
* @version 1.0
* @date 21 Mar 2025
* @author André Vaz (48209) e Filipe Cruz (43468)
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

/** @defgroup BUZZER BUZZER
* @brief Provides the core capabilities for buzzer handling functions.
* @{
*/

#ifndef _BUZZER_H_
#define _BUZZER_H_

/**
 * @brief Initializes the buzzer as a GPIO output.
 */
void BUZZER_Init( void );

/**
 * @brief Turns the buzzer On
 */
void BUZZER_On( void );

/**
 * @brief Turns the buzzer Off
 */
void BUZZER_Off( void );

/**
 * @brief Turns the buzzer on for a specified duration.
 * @param time Represents the duration in milliseconds.
 */
void BUZZER_Alarm( uint32_t time );

#endif /* _BUZZER_H_ */

/**
* @}
*/
/**
* @}
*/
