/**
* @file flashcontrol.h
* @brief Contains the flash control API.
* @version 1.0
* @date 26 Out 2024
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

/** @defgroup FLASHCONTROL FLASHCONTROL
* @brief Provides the core capabilities for flash memory control functions.
* @{
*/

#ifndef _FLASHCONTROL_H_
#define _FLASHCONTROL_H_

/* Status Code messages */
#define COMMAND_SUCCESS_MSG 		"COMMAND_SUCCESS"
#define	INVALID_COMMAND_MSG 		"INVALID_COMMAND"
#define	SRC_ADDR_ERROR_MSG 			"SRC_ADDR_ERROR"
#define	DST_ADDR_ERROR_MSG 			"DST_ADDR_ERROR"
#define	SRC_ADDR_NOT_MAPPED_MSG 	"SRC_ADDR_NOT_MAPPED"
#define	DST_ADDR_NOT_MAPPED_MSG 	"DST_ADDR_NOT_MAPPED"
#define	COUNT_ERROR_MSG 			"COUNT_ERROR"
#define	INVALID_SECTOR_MSG 			"INVALID_SECTOR"
#define	SECTOR_NOT_BLANK_MSG 		"SECTOR_NOT_BLANK"
#define	SECTOR_NOT_PREPARED_MSG 	"SECTOR_NOT_PREPARED"
#define	COMPARE_ERROR_MSG 			"COMPARE_ERROR"
#define	BUSY_MSG 					"BUSY"

/**
* @brief Erases the specified flash memory sector.
* @param sector Flash sector number to erase.
* @return Status code indicating success or error.
*/
int FLASHCONTROL_EraseSector( unsigned int sector );

/**
* @brief Writes data from source to the specified flash destination address.
*
*        Data is padded to the next valid flash write size (256/512/1024/4096 bytes).
*
* @param dstAddr Destination address in flash memory.
* @param srcAddr Source address in RAM.
* @param size Number of bytes to write (must be <= 4096).
*
* @return Status code indicating success or error.
*/
int FLASHCONTROL_WriteData ( void *dstAddr , void *srcAddr , unsigned int size );

/**
* @brief Verifies that the contents of the flash match the source buffer.
*
*        Comparison is word-aligned and may pad the size to the next multiple of 4 bytes.
*
* @param dstAddr Destination flash address.
* @param srcAddr Source buffer address in RAM.
* @param size Number of bytes to compare (up to 4096).
*
* @return Status code indicating success or error.
*/
int FLASHCONTROL_VerifyData ( void *dstAddr , void *srcAddr , unsigned int size );

/**
* @brief Converts a status code into a human-readable message string.
* @param resultValue Status code returned by flash operations.
* @return Pointer to a constant string with the message, or NULL if unknown.
*/
char* FLASHCONTROL_GetResultMsg( int resultValue );

#endif /* _FLASHCONTROL_H_ */

/**
* @}
*/
/**
* @}
*/
