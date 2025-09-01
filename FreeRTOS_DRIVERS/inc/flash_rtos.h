/**
 * @file flash_rtos.h
 * @brief Contains the flash memory FreeRTOS API.
 * @version 1.0
 * @date 13 Jun 2025
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

/** @defgroup FLASH_FreeRTOS FLASH_FreeRTOS
 * @brief Provides the core capabilities for flash memory FreeRTOS functions.
 * @{
 */

#ifndef _FLASH_RTOS_H_
#define _FLASH_RTOS_H_

/**
 * @brief Error code returned by FLASH_RTOS functions when an error occurs.
 */
#define FLASH_RTOS_ERROR   -1

/**
 * @brief Initializes the internal mutex used to protect flash operations.
 *
 * This function must be called before any other FLASH_RTOS function is used.
 */
void FLASH_RTOS_Init(void);

/**
 * @brief Erases the specified flash sector in a thread-safe manner.
 *
 * @param sector The sector number to erase.
 * @return int Result code from FLASHCONTROL_EraseSector or FLASH_RTOS_ERROR if mutex is unavailable.
 */
int FLASH_RTOS_EraseSector(unsigned int sector);

/**
 * @brief Writes data to flash memory in a thread-safe manner.
 *
 * @param dstAddr Destination address in flash memory.
 * @param srcAddr Source address in RAM.
 * @param size Number of bytes to write (up to 4096).
 *
 * @return int Result code from FLASHCONTROL_WriteData or FLASH_RTOS_ERROR if mutex is unavailable.
 */
int FLASH_RTOS_WriteData(void *dstAddr, void *srcAddr, unsigned int size);

/**
 * @brief Verifies flash memory content against a RAM buffer in a thread-safe manner.
 *
 * @param dstAddr Address in flash memory to verify.
 * @param srcAddr Address in RAM to compare with.
 * @param size Number of bytes to verify (up to 4096).
 *
 * @return int Result code from FLASHCONTROL_VerifyData or FLASH_RTOS_ERROR if mutex is unavailable.
 */
int FLASH_RTOS_VerifyData(void *dstAddr, void *srcAddr, unsigned int size);

#endif /* _FLASH_RTOS_H_ */

/**
* @}
*/
/**
* @}
*/
