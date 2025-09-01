/**
* @file adc.h
* @brief Contains the ADC API.
* @version 1.0
* @date 3 Abr 2025
* @author André Vaz (48209) e Filipe Cruz (43468)
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

/** @defgroup ADC ADC
* @brief Provides the core capabilities for ADC configuration functions.
* @{
*/

#ifndef _ADC_H_
#define _ADC_H_


#define ADCR_CLEAN_SEL 	0xFF	/// Mask to clear the channel selection bits in the ADCR register.

#define ADCR_SEL		0		/// Starting bit position for channel selection in the ADCR register.
#define ADCR_CLKDIV		8		/// Bit position for clock divider settings in the ADCR register.
#define ADCR_PDN		21		/// Power-down bit position in the ADCR register.
#define ADCR_START		24		/// Start conversion bit position in the ADCR register.

/**
 * @brief Initializes the ADC on the specified channel.
 *
 * 		  Powers up the ADC peripheral, configures the ADC clock, and sets the appropriate pin function
 * 		  for the selected channel.
 *
 * @param channel ADC channel to initialize (range: 0 to 7).
 *
 */
void ADC_Init(uint8_t channel);

/**
 * @brief Performs a blocking ADC read on the specified channel.
 *
 * 		  Starts a single ADC conversion and waits until the result is ready.
 *
 * @param channel ADC channel to read from (range: 0 to 7).
 *
 * @return 12-bit digital value from the ADC conversion (range: 0 to 4095).
 *
 * @note This function performs a blocking read. It waits until the conversion is complete.
 */
uint16_t ADC_Read(uint8_t channel);

#endif /* _ADC_H_ */

/**
* @}
*/
/**
* @}
*/

