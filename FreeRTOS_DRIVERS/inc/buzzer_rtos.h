/**
* @file buzzer_rtos.h
* @brief Contains the Buzzer FreeRTOS API.
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

/** @defgroup FreeRTOS_DRIVERS FreeRTOS_DRIVERS
* @brief This package provides FreeRTOS drivers layer functions.
* @{
*/

/** @defgroup BUZZER_FreeRTOS BUZZER_FreeRTOS
* @brief Provides the core capabilities for Buzzer FreeRTOS related functions.
* @{
*/

#ifndef _BUZZER_RTOS_H_
#define _BUZZER_RTOS_H_

/**
 * @enum BUZZER_Command
 * @brief Enumeration representing buzzer command types.
 *
 *        This enum defines the possible actions that can be sent
 *        to the buzzer task, such as turning it ON, OFF or triggering an alarm.
 */
typedef enum {
    BUZZER_CMD_ON = 0x01,     /**< Turn the buzzer ON. */
    BUZZER_CMD_OFF = 0x02,    /**< Turn the buzzer OFF. */
    BUZZER_CMD_ALARM = 0x03   /**< Trigger an alarm for a specific duration. */
} BUZZER_Command;

/**
 * @brief Initializes the buzzer task.
 *
 *        This function creates the task responsible for controlling the buzzer state
 *        (on, off, or alarm) based on received notifications.
 *
 * @note This function must be called before accessing any Buzzer-related function.
 *
 */
void BUZZER_RTOS_Init( void );

/**
 * @brief FreeRTOS task responsible for controlling the buzzer.
 *
 *        The task waits for notifications containing commands to turn the buzzer
 *        on, off, or activate alarm mode.
 *
 * @param pvParameters: Task parameter (not used).
 *
 * @note The task must be initialized using "BUZZER_RTOS_Init()".
 *
 */
void BUZZER_RTOS_Task( void *pvParameters );

bool BUZZER_RTOS_Ready( void );

/**
 * @brief Sends command to turn the buzzer on.
 *
 *        This function notifies the buzzer task to turn it on.
 *
 */
void BUZZER_RTOS_On( void );

/**
 * @brief Sends command to turn the buzzer off.
 *
 *        This function notifies the buzzer task to turn it off.
 *
 */
void BUZZER_RTOS_Off( void );

/**
 * @brief Sends command to activate the buzzer alarm mode.
 *
 *        This function notifies the buzzer task to activate alarm mode
 *        for a specified duration.
 *
 * @param duration: Alarm duration (milliseconds).
 *
 */
void BUZZER_RTOS_Alarm( uint32_t duration );

#endif /* _BUZZER_RTOS_H_ */

/**
* @}
*/
/**
* @}
*/
