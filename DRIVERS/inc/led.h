/**
* @file led.h
* @brief Contains the led API.
* @version 1.0
* @date 25 Fev 2025
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

/** @defgroup LED LED
* @brief Provides the core capabilities for led related functions.
* @{
*/

#ifndef _LED_H_
#define _LED_H_

#include <stdbool.h>

/**
 * @typedef LedModelEnum
 * @brief Enumeration of LED driving logic models.
 *
 * 		  Defines how the LED is controlled electrically.
 */
typedef enum LedModelEnum{
	LED_MODEL_ACTIVE_HIGH, /**< LED is ON when output is HIGH */
	LED_MODEL_ACTIVE_LOW   /**< LED is ON when output is LOW */
} LED_ModelType;

/**
 * @typedef LedColorEnum
 * @brief Enumeration of LED colors and types.
 *
 * 		  Represents the color and category of LEDs used in the system.
 */
typedef enum LedColorEnum {
	LED_INT_RED,
	LED_INT_GREEN,
	LED_INT_BLUE,
	LED_EXT_RED,
	LED_EXT_GREEN,
	LED_EXT_BLUE,
	LED_SEM_RED,
	LED_SEM_YELLOW,
	LED_SEM_GREEN,
} LED_ColorValueType;

/**
 * @brief Initializes an LED with the specified color, initial state, and control model.
 * @param ledColorId The color identifier of the LED to initialize.
 * @param state The initial state of the LED (true for ON, false for OFF).
 * @param model The LED control model (active-high or active-low).
 */
void LED_Init ( LED_ColorValueType ledColorId, bool state, LED_ModelType model );

/**
 * @brief Gets the current state of the specified LED.
 * @param ledColorId The color identifier of the LED.
 * @return true if the LED is ON; false otherwise.
 */
bool LED_GetState( LED_ColorValueType ledColorId );

/**
 * @brief Turns ON the specified LED according to its control model.
 * @param ledColorId The color identifier of the LED.
 * @param model The LED control model (active-high or active-low).
 */
void LED_On( LED_ColorValueType ledColorId, LED_ModelType model );

/**
 * @brief Turns OFF the specified LED according to its control model.
 * @param ledColorId The color identifier of the LED.
 * @param model The LED control model (active-high or active-low).
 */
void LED_Off ( LED_ColorValueType ledColorId, LED_ModelType model );

/**
 * @brief Toggles the state of the specified LED (ON to OFF or OFF to ON).
 * @param ledColorId The color identifier of the LED.
 * @param model The LED control model (active-high or active-low).
 */
void LED_Toggle ( LED_ColorValueType ledColorId, LED_ModelType model );

/**
 * @brief Returns the hardware pin number associated with the specified LED color.
 * @param ledColorId The color identifier of the LED.
 * @return The hardware pin number corresponding to the LED.
 */
int LED_GetLedByColor ( LED_ColorValueType ledColorId );

#endif /* _LED_H_ */

/**
* @}
*/
/**
* @}
*/
