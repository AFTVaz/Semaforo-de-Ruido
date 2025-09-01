/**
 * @file microphone.h
 * @brief Contains the MAX9814 FreeRTOS API.
 * @version 1.0
 * @date 4 Jun 2025
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

/** @defgroup MICROPHONE MICROPHONE
 * @brief Provides the core capabilities for MAX9814 FreeRTOS functions.
 * @{
 */

#ifndef _MICROPHONE_H_
#define _MICROPHONE_H_

#include "max9814.h"

/**
 * @brief Initializes the microphone module and creates the control mutex.
 *
 *        This function creates a mutex to ensure thread-safe access to the microphone hardware.
 *        It then initializes the MAX9814 driver with the specified gain configuration.
 *        The gain can be changed later using MICROPHONE_SetGain().
 *
 * @param gain Initial gain setting for the MAX9814 microphone.
 *             Possible values:
 *             - MAX9814_VCC_GAIN (40 dB)
 *             - MAX9814_GND_GAIN (50 dB)
 *             - MAX9814_FLOAT_GAIN (60 dB)
 *
 * @note This function must be called before using any other MICROPHONE-related function.
 */
void MICROPHONE_Init( MAX9814_GainValueType gain );

/**
 * @brief Reads the current sound level in decibels (dB).
 *
 *        This function collects multiple ADC samples from the microphone,
 *        computes the RMS voltage, and converts it into a decibel value.
 *        Access to the microphone is controlled by a mutex to avoid concurrent interference.
 *
 * @return Current sound level in decibels (dB).
 * @retval -INFINITY If the RMS voltage is zero or below detection threshold.
 * @retval >0 Valid sound level including microphone gain compensation.
 *
 * @note The function blocks execution until the mutex is available for safe reading.
 */
float MICROPHONE_Read( void );

/**
 * @brief Changes the gain level of the microphone.
 *
 *        This function adjusts the analog gain of the MAX9814 microphone by configuring
 *        the gain control pin. It also updates the internal dB compensation value
 *        used when converting voltage to decibels.
 *        The access is protected by a mutex to ensure safe concurrent operation.
 *
 * @param gain Gain setting to apply. Options:
 *             - MAX9814_VCC_GAIN (40 dB)
 *             - MAX9814_GND_GAIN (50 dB)
 *             - MAX9814_FLOAT_GAIN (60 dB)
 *
 * @note The function blocks execution until the mutex is available for safe configuration.
 */
void MICROPHONE_SetGain( MAX9814_GainValueType gain );

#endif /* _MICROPHONE_H_ */

/**
* @}
*/
/**
* @}
*/
