/**
* @file led_rtos.h
* @brief Contains the LED FreeRTOS API.
* @version 1.0
* @date 17 Mar 2025
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

/** @defgroup LED_FreeRTOS LED_FreeRTOS
* @brief Provides the core capabilities for LED FreeRTOS related functions.
* @{
*/

#ifndef _LED_RTOS_H_
#define _LED_RTOS_H_

#include "led.h"

#define LED_MAX_MUTEX_RETRY		5

#define LED_GETSTATE_DELAY		pdMS_TO_TICKS(20)
#define LED_GETSTATE_TIMEOUT 	pdMS_TO_TICKS(1000)

/**
 * @enum LED_Command
 * @brief Enumeration representing LED operation types for queued commands.
 *
 * 		  This enum defines the possible actions that can be queued
 * 		  to control an LED (turn it on, off, or toggle its state).
 *
 */
typedef enum {
    LED_CMD_ON,     /**< Command to turn the LED ON. */
    LED_CMD_OFF,    /**< Command to turn the LED OFF. */
    LED_CMD_TOGGLE  /**< Command to TOGGLE the LED state. */
} LED_Command;

/**
 * @struct LED_RTOS_Command
 * @brief Structure representing a command to be executed by the LED task.
 */
typedef struct {
    LED_ColorValueType color;	/**< LED color to operate */
    LED_ModelType model;		/**< LED model (internal/external/semaphore) */
    LED_Command command;		/**< LED operation to perform */
} LED_RTOS_Command;

/**
 * @brief Checks whether the LED RTOS task and queue have been initialized successfully.
 *
 * @return true if ready; false otherwise.
 */
bool LED_RTOS_Ready( void );

/**
 * @brief Initializes the internal red LED and creates the LED control task.
 *
 * @param state Initial state of the LED (true = on, false = off).
 * @param model LED model type.
 *
 * @note Should be called during system initialization if only the red internal LED is used.
 */
void LED_RTOS_InitRedInt( bool state, LED_ModelType model );

/**
 * @brief Initializes all internal RGB LEDs and creates the LED control task.
 *
 * @param state Initial state of all LEDs (true = on, false = off).
 * @param model LED model type.
 *
 * @note This function sets up red, green, and blue internal LEDs.
 */
void LED_RTOS_InitAllInt(bool state, LED_ModelType model);

/**
 * @brief Initializes all external RGB LEDs and creates the LED control task.
 *
 * @param state Initial state of all LEDs (true = on, false = off).
 * @param model LED model type.
 *
 * @note This function sets up red, green, and blue external LEDs.
 */
void LED_RTOS_InitAllExt(bool state, LED_ModelType model);

/**
 * @brief Initializes the semaphore-style traffic light LED set and creates the LED control task.
 *
 * @param model LED model type.
 *
 * @note Initializes red and yellow LEDs to OFF and green to ON by default.
 */
void LED_RTOS_InitSemaphore(LED_ModelType model);

/**
 * @brief Gets the current state of a specific LED.
 *
 * 		  This function retrieves the state of the LED corresponding to the given color.
 * 		  It uses a mutex to ensure thread-safe access. If the mutex cannot be obtained,
 * 		  the function returns false and the state is not updated.
 *
 * @param color The color of the LED to query.
 * @param state Pointer to a boolean variable to store the LED state.
 *              Will be set to true if the LED is ON, or false if OFF.
 *
 * @retval true  If the state was successfully retrieved.
 * @retval false If the mutex could not be acquired or state retrieval failed.
 */
bool LED_RTOS_GetState(LED_ColorValueType color, bool *state);

/**
 * @brief Turns on the specified LED asynchronously using the LED task.
 *
 * @param color LED color to turn on.
 * @param model LED model type.
 */
void LED_RTOS_On(LED_ColorValueType color, LED_ModelType model);

/**
 * @brief Turns off the specified LED asynchronously using the LED task.
 *
 * @param color LED color to turn off.
 * @param model LED model type.
 */
void LED_RTOS_Off(LED_ColorValueType color, LED_ModelType model);

/**
 * @brief Toggles the state of the specified LED asynchronously using the LED task.
 *
 * @param color LED color to toggle.
 * @param model LED model type.
 */
void LED_RTOS_Toggle(LED_ColorValueType color, LED_ModelType model);

/**
 * @brief Shuts down and cleans up the LED RTOS module.
 *
 * 		  Deletes the LED task, queue, and mutex.
 */
void LED_RTOS_Delete(void);


#endif /* _LED_RTOS_H_ */

/**
* @}
*/
/**
* @}
*/
