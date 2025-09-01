/**
* @file pin.h
* @brief Contains the pin API.
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

/** @defgroup PIN PIN
* @brief Provides the core capabilities for pin related functions.
* @{
*/

#ifndef _PIN_H_
#define _PIN_H_

#include <stdbool.h>

/* Encoder pins */
#define ENCODER_PIN_A 		P0_3		/* CLK */
#define ENCODER_PIN_B 		P0_2		/* DT */
#define ENCODER_BUTTON 		P1_31		/* SW */

/* Buzzer pins */
#define BUZZER_PIN 			P2_3

/* LED pins */
#define LED_RED_INTERNAL 	P0_22
#define LED_GREEN_INTERNAL 	P3_25
#define LED_BLUE_INTERNAL 	P3_26
#define LED_RED_EXTERNAL 	P2_3
#define LED_GREEN_EXTERNAL 	P2_4
#define LED_BLUE_EXTERNAL 	P2_5
#define LED_RED_SEM			P2_10
#define LED_YELLOW_SEM		P2_11
#define LED_GREEN_SEM		P2_12

/* ADC pins */
#define ADC_AD00_PIN 		P0_23
#define ADC_AD01_PIN 		P0_24
#define ADC_AD02_PIN 		P0_25
#define ADC_AD03_PIN 		P0_26
#define ADC_AD04_PIN 		P1_30
#define ADC_AD05_PIN 		P1_31
#define ADC_AD06_PIN		P0_2
#define ADC_AD07_PIN 		P0_3

/* MAX9814 pins */
#define MAX9814_GAIN_PIN	P0_21

/*! \var typedef enum PIN_ValueEnum
    \brief Pin name enumeration.
*/
typedef enum PIN_ValueEnum{
	P0_0 = (0 * 32 + 0),
	P0_1 = (0 * 32 + 1),
	P0_2 = (0 * 32 + 2),
	P0_3 = (0 * 32 + 3),
	P0_4 = (0 * 32 + 4),
	P0_5 = (0 * 32 + 5),
	P0_6 = (0 * 32 + 6),
	P0_7 = (0 * 32 + 7),
	P0_8 = (0 * 32 + 8),
	P0_9 = (0 * 32 + 9),
	P0_10 = (0 * 32 + 10),
	P0_11 = (0 * 32 + 11),
	//P0_12 = (0 * 32 + 12),		//RESERVED
	//P0_13 = (0 * 32 + 13),		//RESERVED
	//P0_14 = (0 * 32 + 14),		//RESERVED
	P0_15 = (0 * 32 + 15),
	P0_16 = (0 * 32 + 16),
	P0_17 = (0 * 32 + 17),
	P0_18 = (0 * 32 + 18),
	P0_19 = (0 * 32 + 19),
	P0_20 = (0 * 32 + 20),
	P0_21 = (0 * 32 + 21),
	P0_22 = (0 * 32 + 22),
	P0_23 = (0 * 32 + 23),
	P0_24 = (0 * 32 + 24),
	P0_25 = (0 * 32 + 25),
	P0_26 = (0 * 32 + 26),
	P0_27 = (0 * 32 + 27),
	P0_28 = (0 * 32 + 28),
	P0_29 = (0 * 32 + 29),
	P0_30 = (0 * 32 + 30),

	P1_0 = (1 * 32 + 0),
	P1_1 = (1 * 32 + 1),
	P1_2 = (1 * 32 + 2),
	P1_3 = (1 * 32 + 3),
	//P1_4 = (1 * 32 + 4),		//RESERVED
	//P1_5 = (1 * 32 + 5),		//RESERVED
	//P1_6 = (1 * 32 + 6),		//RESERVED
	//P1_7 = (1 * 32 + 7),		//RESERVED
	P1_8 = (1 * 32 + 8),
	P1_9 = (1 * 32 + 9),
	//P1_10 = (1 * 32 + 10),	//RESERVED
	//P1_11 = (1 * 32 + 11),	//RESERVED
	//P1_12 = (1 * 32 + 12),	//RESERVED
	//P1_13 = (1 * 32 + 13),	//RESERVED
	//P1_14 = (1 * 32 + 14),	//RESERVED
	//P1_15 = (1 * 32 + 15),	//RESERVED
	P1_16 = (1 * 32 + 16),
	P1_17 = (1 * 32 + 17),
	P1_18 = (1 * 32 + 18),
	P1_19 = (1 * 32 + 19),
	P1_20 = (1 * 32 + 20),
	P1_21 = (1 * 32 + 21),
	//P1_22 = (1 * 32 + 22),	//RESERVED
	//P1_23 = (1 * 32 + 23),	//RESERVED
	//P1_24 = (1 * 32 + 24),	//RESERVED
	//P1_25 = (1 * 32 + 25),	//RESERVED
	//P1_26 = (1 * 32 + 26),	//RESERVED
	//P1_27 = (1 * 32 + 27),	//RESERVED
	P1_28 = (1 * 32 + 28),
	P1_29 = (1 * 32 + 29),
	P1_30 = (1 * 32 + 30),
	P1_31 = (1 * 32 + 31),

	P2_0 = (2 * 32 + 0),
	P2_1 = (2 * 32 + 1),
	P2_2 = (2 * 32 + 2),
	P2_3 = (2 * 32 + 3),
	P2_4 = (2 * 32 + 4),
	P2_5 = (2 * 32 + 5),
	P2_6 = (2 * 32 + 6),
	P2_7 = (2 * 32 + 7),
	P2_8 = (2 * 32 + 8),
	P2_9 = (2 * 32 + 9),
	P2_10 = (2 * 32 + 10),
	P2_11 = (2 * 32 + 11),
	P2_12 = (2 * 32 + 12),
	P2_13 = (2 * 32 + 13),
	P2_14 = (2 * 32 + 14),
	P2_15 = (2 * 32 + 15),
	P2_16 = (2 * 32 + 16),
	P2_17 = (2 * 32 + 17),
	P2_18 = (2 * 32 + 18),
	P2_19 = (2 * 32 + 19),
	P2_20 = (2 * 32 + 20),
	P2_21 = (2 * 32 + 21),
	P2_22 = (2 * 32 + 22),
	P2_23 = (2 * 32 + 23),
	P2_24 = (2 * 32 + 24),
	P2_25 = (2 * 32 + 25),
	P2_26 = (2 * 32 + 26),
	P2_27 = (2 * 32 + 27),
	//P2_28 = (2 * 32 + 28),	//RESERVED
	//P2_29 = (2 * 32 + 29),	//RESERVED
	//P2_30 = (2 * 32 + 30),	//RESERVED
	//P2_31 = (2 * 32 + 31)		//RESERVED

	P3_25 = (3 * 32 + 25),
	P3_26 = (3 * 32 + 26),

	P4_28 = (4 * 32 + 28),
	P4_29 = (4 * 32 + 29),

}PIN_ValueType;

/*! \var typedef enum PIN_PortEnum
    \brief Port name enumeration.
*/
typedef enum PIN_PortEnum{
	Port0 = 0,
	Port1 = 1,
	Port2 = 2,
	Port3 = 3
}PIN_PortType;

/*! \var typedef enum PIN_DirectionEnum
    \brief Pin direction enumeration.
*/
typedef enum PIN_DirectionEnum{
	INPUT, OUTPUT
}PIN_DIRType;

/**
 * @brief Configures the direction of the specified pin.
 *
 * @param pinId The identifier of the pin.
 * @param dir The desired direction of the pin: INPUT or OUTPUT.
 */
void PIN_Direction( PIN_ValueType pinId, PIN_DIRType dir );

/**
 * @brief Sets the specified pin to logical HIGH.
 *
 * @param pinId The identifier of the pin to set.
 */
void PIN_Set( PIN_ValueType pinId );

/**
 * @brief Clears the specified pin to logical LOW.
 *
 * @param pinId The identifier of the pin to clear.
 */
void PIN_Clear( PIN_ValueType pinId );

/**
 * @brief Reads the current logical state of the specified pin.
 *
 * @param pinId The identifier of the pin to read.
 * @return uint32_t The current value of the pin (non-zero if HIGH, zero if LOW).
 */
uint32_t PIN_Value( PIN_ValueType pinId );

/**
 * @brief Writes a masked value to the entire GPIO port.
 *
 * 		  Only bits specified by the mask are affected; others remain unchanged.
 *
 * @param portId The identifier of the port (Port0, Port1, Port2, Port3).
 * @param value The bits to write on the port (only bits in mask apply).
 * @param mask A bitmask selecting which bits in the port are modified.
 */
void PIN_PortWrite( PIN_PortType portId, uint32_t value, uint32_t mask );

#endif /* _PIN_H_ */

/**
* @}
*/
/**
* @}
*/
