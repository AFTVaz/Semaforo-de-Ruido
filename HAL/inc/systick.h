/**
* @file systick.h
* @brief Contains the systick API.
* @version 1.1
* @date 10 Out 2017
* @author PSampaio
*
* Copyright(C) 2015-2023, PSampaio
* All rights reserved.
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

/** @defgroup SYSTICK SYSTICK
* @brief Provides the core capabilities for system timer functions.
* @{
*/


#ifndef _SYSTICK_H_
#define _SYSTICK_H_

/**
* @brief Initialises the systick API for 1 ms resolution.
* @return 0 if initialisation successed; -1 if fails.
* @note This function must be called prior to any other SYSTICK
* functions, and use the SYSTICK resource.
*/
int SYSTICK_Init(uint32_t value);


/**
* @brief Get current value of ticks
* @return Returns the current tick value in milliseconds.
*/
uint32_t SYSTICK_GetValue(void);

#endif /* _SYSTICK_H_ */

/**
* @}
*/
/**
* @}
*/
