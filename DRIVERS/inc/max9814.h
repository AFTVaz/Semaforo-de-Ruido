/**
* @file max9814.h
* @brief Contains the microphone MAX9814 API.
* @version 1.0
* @date 16 Abr 2025
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

/** @defgroup MAX9814 MAX9814
* @brief Provides the core capabilities for microphone MAX9814 related functions.
* @{
*/

#ifndef _MAX9814_H_
#define _MAX9814_H_

/**
 * @enum MAX9814_GainValueType
 * @brief Enumeration for selecting the analog gain of the MAX9814 microphone module.
 *
 *        This enum defines the gain levels that can be applied to the MAX9814 by
 *        configuring its gain selection pin. Each level corresponds to a specific
 *        hardware wiring: VCC, GND, or floating input.
 *
 */
typedef enum MAX9814_GainValueType {
    MAX9814_VCC_GAIN = 40,   /**< Gain = 40 dB (Gain pin connected to VCC)      */
    MAX9814_GND_GAIN = 50,   /**< Gain = 50 dB (Gain pin connected to GND)      */
    MAX9814_FLOAT_GAIN = 60  /**< Gain = 60 dB (Gain pin left floating/open)    */
} MAX9814_GainValueType;

/**
 * @brief Initializes the MAX9814 microphone module.
 *
 *        This function sets up the ADC input channel, configures the gain selection pin,
 *        and performs offset calibration for the ADC to ensure accurate signal centering.
 *        It must be called before any sound level measurements are taken.
 *
 * @param mic_gain Initial gain level for the MAX9814 microphone.
 *                 Acceptable values:
 *                 - MAX9814_VCC_GAIN (40 dB)
 *                 - MAX9814_GND_GAIN (50 dB)
 *                 - MAX9814_FLOAT_GAIN (60 dB)
 *
 * @note Requires that ADC and GPIO configuration functions are properly implemented.
 */
void MAX9814_Init( MAX9814_GainValueType mic_gain );

/**
 * @brief Reads the current ambient sound level in decibels (dB SPL approximation).
 *
 *        This function captures multiple ADC samples, calculates the RMS (Root Mean Square)
 *        value of the signal, and converts it to decibels. The calculation also includes
 *        internal compensation for the configured analog gain.
 *
 *        It also performs automatic gain control (AGC) by adjusting the hardware gain
 *        based on configurable dB thresholds with hysteresis.
 *
 * @return Sound pressure level in decibels (dB SPL approximation).
 * @retval -INFINITY If the signal level is too low to compute a valid dB value.
 * @retval >0 Valid sound level in dB.
 */
float MAX9814_Read( void );

/**
 * @brief Updates the analog gain setting of the MAX9814 microphone module.
 *
 *		  This function sets the appropriate logic level on the GAIN pin to configure
 *		  the microphone's analog gain (40 dB, 50 dB, or 60 dB). It also updates the
 *		  internal compensation factor for dB conversion and recalculates the reference
 *		  output voltage based on the selected gain.
 *
 * @param mic_gain New gain level to apply.
 * 				   Options:
 *                 		- MAX9814_VCC_GAIN (40 dB)
 *                 		- MAX9814_GND_GAIN (50 dB)
 *                 		- MAX9814_FLOAT_GAIN (60 dB)
 *
 */
void MAX9814_SetGain( MAX9814_GainValueType mic_gain );

#endif /* _MAX9814_H_ */

/**
* @}
*/
/**
* @}
*/
