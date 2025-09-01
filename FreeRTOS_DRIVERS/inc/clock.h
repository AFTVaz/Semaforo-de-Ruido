/**
* @file clock.h
* @brief Contains the RTC FreeRTOS API.
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

/** @defgroup CLOCK CLOCK
* @brief Provides the core capabilities for RTC FreeRTOS functions.
* @{
*/

#ifndef _CLOCK_H_
#define _CLOCK_H_

/**
 * @brief Initializes the RTC module and creates the control mutex.
 *
 *        This function checks if the RTC mutex has been properly created, and if not,
 *        it creates it before initializing the RTC. The mutex is used to protect
 *        concurrent access to the RTC.
 *
 * @note This function must be called before accessing any RTC-related function.
 *
 */
void CLOCK_Init( void );

/**
 * @brief Retrieves the current date and time from the RTC.
 *
 *        The function uses a mutex to ensure exclusive access to the RTC,
 *        preventing interference from other tasks. The date and time values
 *        are filled into the provided "struct tm".
 *
 * @param dateTime Pointer to the "struct tm" structure where date and time will be stored.
 *
 * @note The function blocks execution until the mutex is available for safe reading.
 */
void CLOCK_GetTimeDate( struct tm *dateTime );

/**
 * @brief Sets the date and time in the RTC.
 *
 *        This function sets the system date and time, protecting access to the RTC with a mutex.
 *        It ensures that only one task has exclusive access to the RTC during the operation.
 *
 * @param dateTime Pointer to the "struct tm" structure containing the date and time to be set.
 *
 * @note The function blocks execution until the mutex is available for safe writing.
 */
void CLOCK_SetTimeDate( struct tm *dateTime );

/**
 * @brief Sets the absolute time in seconds since 1970 (Epoch).
 *
 *        This function updates the RTC with the provided value in seconds.
 *        Access is protected by a mutex to ensure no other task interferes during the update.
 *
 * @param seconds Absolute time in seconds since 1970-01-01 (Epoch).
 *
 * @note The function blocks execution until the mutex is available for safe writing.
 */
void CLOCK_SetSeconds( time_t seconds );

/**
 * @brief Gets the current RTC time in seconds since 1970 (Epoch).
 *
 *        Returns the current time value in seconds since the Unix Epoch (1970-01-01).
 *        Read access is protected by a mutex to prevent interference from other tasks.
 *
 * @return Current time in seconds since 1970-01-01.
 * @retval 0 If the RTC is at the initial date or not initialized.
 * @retval >0 Valid time value.
 *
 * @note The function blocks execution until the mutex is available for safe reading.
 */
time_t CLOCK_GetSeconds( void );

#endif /* _CLOCK_H_ */

/**
* @}
*/
/**
* @}
*/
