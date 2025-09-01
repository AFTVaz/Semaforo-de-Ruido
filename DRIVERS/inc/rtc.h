/**
* @file rtc.h
* @brief Contains the rtc API.
* @version 1.0
* @date 5 Nov 2024
* @author André Vaz (48209) e Emilio Mendes (48170)
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

/** @defgroup RTC RTC
* @brief Provides the core capabilities for rtc functions.
* @{
*/

#ifndef _RTC_H_
#define _RTC_H_

/* RTC configuration values */
#define RTC_PCONP_ENABLE 	9			/**< Power Control bit position for RTC in PCONP */
#define CLKEN_BIT_POS 		0			/**< Bit position for clock enable in CCR */
#define CTIME_SEC_MASK 		0x3F		/**< Mask for seconds in CTIME0 (bits 0–5) */
#define CTIME_MIN_MASK 		0x3F00		/**< Mask for minutes in CTIME0 (bits 8–13) */
#define CTIME_HOUR_MASK 	0x1F0000	/**< Mask for hours in CTIME0 (bits 16–20) */
#define CTIME_DOW_MASK 		0x7000000	/**< Mask for day of week in CTIME0 (bits 24–26) */
#define CTIME_DOM_MASK 		0x1F		/**< Mask for day of month in CTIME1 (bits 0–4) */
#define CTIME_MONTH_MASK 	0xF00		/**< Mask for month in CTIME1 (bits 8–11) */
#define CTIME_YEAR_MASK 	0xFFF0000	/**< Mask for year in CTIME1 (bits 16–27) */
#define CTIME_DOY_MASK 		0xFFF		/**< Mask for day of year in CTIME2 (bits 0–11) */

/**
 * @brief Compares the time (hours, minutes, seconds) of two struct tm values.
 *
 * 		  Compares only the time portion (ignores date) between structures.
 *
 * @param a Pointer to time structure A.
 * @param b Pointer to time structure B.
 *
 * @return Integer result: negative if A < B, positive if A > B, zero if equal.
 */
int compareByTime(const struct tm *a, const struct tm *b);


/**
 * @brief Initializes the RTC with a timestamp (seconds since 1970-01-01 00:00).
 *
 * 		  Enables RTC power, disables the clock during setup, sets time,
 * 	  	  and then starts the RTC clock.
 *
 * @param seconds Seconds since Unix epoch (1970-01-01 00:00).
 */
void RTC_Init(time_t seconds);


/**
 * @brief Retrieves current date and time from the RTC.
 *
 * 		  Populates a given struct tm with the current values read from CTIME registers.
 *
 * @param dateTime Pointer to a struct tm to be filled with RTC values.
 */
void RTC_GetTimeDate(struct tm *dateTime);


/**
 * @brief Sets the RTC with a full date and time from a struct tm.
 *
 * 		  Attempts to apply the desired date/time repeatedly until RTC reflects it.
 * 		  The clock is stopped during updates and re-enabled after.
 *
 * @param dateTime Pointer to struct tm containing desired time and date.
 */
void RTC_SetTimeDate(struct tm *dateTime);


/**
 * @brief Sets the RTC time using seconds since the Unix epoch (1970-01-01 00:00).
 *
 * 		  Converts the seconds to a struct tm using localtime() and sets the RTC.
 *
 * @param seconds Seconds since Unix epoch.
 */
void RTC_SetSeconds(time_t seconds);


/**
 * @brief Gets the number of seconds since 1970-01-01 00:00 from RTC.
 *
 * 		  Reads current RTC time, fills struct tm, and converts it to time_t.
 *
 * @return Seconds since Unix epoch.
 */
time_t RTC_GetSeconds(void);

#endif /* _RTC_H_ */

/**
* @}
*/
/**
* @}
*/
