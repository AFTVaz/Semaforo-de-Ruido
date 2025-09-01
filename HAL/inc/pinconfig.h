/**
* @file pinconfig.h
* @brief Contains the pinconfig API.
* @version 1.0
* @date 1 Out 2017
* @author André Vaz (48209) e Emilio Mendes (48170)
*
*
**********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
*********************************************************************/

/** @defgroup HAL HAL
* @brief This package provides hardware abstraction layer functions.
* @{
*/

/** @defgroup PINCONFIG PINCONFIG
* @brief Provides the core capabilities for pin configuration functions.
* @{
*/

#ifndef _PINCONFIG_H_
#define _PINCONFIG_H_

#include "pin.h"

/*! \def LPC_GPIO_NUM_PINS
    \brief Number of GPIO pins per port on the LPC microcontroller (32 pins).
 */
#define LPC_GPIO_NUM_PINS 32

/*! \enum PINCFG_FunctionEnum
    \brief Enumeration of possible functions for a pin.

    Each pin can be configured for a specific function such as GPIO, UART, SPI, I2C, PWM, ADC, etc.
    The numeric value corresponds to the PINSEL register configuration.
 */
typedef enum PINCFG_FunctionEnum {
	GPIO = 0,
	UART1 = 1,
	UART1_ALT = 2,
	UART2 = 1,
	UART3 = 2,
	UART3_ALT = 3,
	SPI = 3,
	SSP0 = 2,
	I2C0 = 1,
	I2C1 = 3,
	I2C1_ALT = 3,
	I2C2 = 2,
	PWM_F1 = 1,
	PWM_F2 = 2,
	PWM_F3 = 3,
	AD0_F1 = 1,
	AD0_F2 = 2,
	AD0_F3 = 3
} PINCFG_FunctionType;

/*! \enum PINCFG_ResistorModeEnum
    \brief Enumeration for internal resistor modes of the pin.

    Defines the internal resistor mode: pull-up, pull-down, repeater, or none.
 */
typedef enum PINCFG_ResistorModeEnum {
	PULL_UP,    /**< Internal pull-up resistor enabled */
	REPEATER,   /**< Repeater mode */
	NONE,       /**< No internal resistor */
	PULL_DOWN   /**< Internal pull-down resistor enabled */
} PINCFG_ResistorModeType;

/*! \enum PINCFG_OpenDrainModeEnum
    \brief Enumeration for the open-drain mode of the pin.

    Defines if the pin operates in normal or open-drain mode.
 */
typedef enum PINCFG_OpenDrainModeEnum {
	NORMAL,     /**< Normal mode */
	OPEN_DRAIN  /**< Open-drain mode enabled */
} PINCFG_OpenDrainModeType;

/**
 * @brief Sets the function of the specified pin (e.g., GPIO, UART, SPI, etc.)
 *
 * 		  This function updates the appropriate bits in the PINSEL registers for the given pin.
 *
 * @param pinId Pin identifier
 * @param func Function to assign to the pin
 */
void PINCFG_Function(PIN_ValueType pinId, PINCFG_FunctionType func);

/**
 * @brief Configures the internal resistor mode of the specified pin.
 *
 * 		  This sets the corresponding bits in the PINMODE registers to enable pull-up, pull-down, repeater, or no resistor.
 *
 * @param pinId Pin identifier
 * @param mode Resistor mode to set
 */
void PINCFG_Mode(PIN_ValueType pinId, PINCFG_ResistorModeType mode);

/**
 * @brief Configures the open-drain mode of the specified pin.
 *
 * 		  This updates the bits in the PINMODE_OD registers to enable or disable open-drain mode.
 *
 * @param pinId Pin identifier
 * @param mode Open-drain mode to set (NORMAL or OPEN_DRAIN)
 */
void PINCFG_OpenDrainMode(PIN_ValueType pinId, PINCFG_OpenDrainModeType mode);

#endif /* _PINCONFIG_H_ */

/**
* @}
*/
/**
* @}
*/
