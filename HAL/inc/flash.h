/**
* @file flash.h
* @brief Contains the flash API.
* @version 1.0
* @date 19 Nov 2024
* @author André Vaz (48209) e Emilio Mendes (48170)
*
*
**********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
*********************************************************************/

/** @defgroup HAL HAL
* @brief This package provides drivers layer functions.
* @{
*/

/** @defgroup FLASH FLASH
* @brief Provides the core capabilities for flash functions.
* @{
*/

#ifndef _FLASH_H_
#define _FLASH_H_

/**
 * @enum FLASH_StatusCodeType
 * @brief Return status codes from IAP Flash operations.
 */
typedef enum {
    CMD_SUCCESS,                                 /**< Operation successful */
    INVALID_COMMAND,                             /**< Invalid IAP command code */
    SRC_ADDR_ERROR,                              /**< Source address error (not on word boundary) */
    DST_ADDR_ERROR,                              /**< Destination address error (not on word boundary) */
    SRC_ADDR_NOT_MAPPED,                         /**< Source address not mapped in memory */
    DST_ADDR_NOT_MAPPED,                         /**< Destination address not mapped in memory */
    COUNT_ERROR,                                 /**< Byte count is not valid */
    INVALID_SECTOR,                              /**< Sector number is invalid or out of range */
    SECTOR_NOT_BLANK,                            /**< Sector is not blank */
    SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION,     /**< Sector not prepared before writing */
    COMPARE_ERROR,                               /**< Compare operation failed (data mismatch) */
    BUSY                                         /**< Flash is busy with another operation */
} FLASH_StatusCodeType;

/**
 * @enum FLASH_CommandCodeType
 * @brief IAP command codes for Flash operations.
 */
typedef enum {
    Prepare_Sectors_For_Write = 50,      /**< Prepare sectors for write/erase */
    COPY_RAM_TO_FLASH = 51,              /**< Copy data from RAM to Flash */
    Erase_Sectors = 52,                  /**< Erase Flash sectors */
    Blank_Check_Sectors = 53,            /**< Check if sectors are blank */
    Read_Part_Identification_Number = 54,/**< Read part ID number */
    Read_Boot_Code_Version_Number = 55,  /**< Read boot code version */
    Compare_Addr1_Addr2_nBytes = 56,     /**< Compare memory contents */
    Re_Invoke_ISP = 57,                  /**< Reinvoke ISP mode */
    Read_Device_Serial_Number = 58       /**< Read device serial number */
} FLASH_CommandCodeType;

/**
 * @brief Erases a given Flash sector.
 * @param sector Sector number to erase (e.g., 0 to 29 for LPC1769)
 * @return Status code indicating success or specific error.
 */
unsigned int FLASH_EraseSector(unsigned int sector);

/**
 * @brief Writes data from RAM to a given Flash address.
 * @param dstAddr Destination Flash address (must be 256-byte aligned)
 * @param srcAddr Source RAM address (must be word-aligned)
 * @param size Number of bytes to write (must be multiple of 256)
 * @return Status code indicating success or specific error.
 */
unsigned int FLASH_WriteData(void *dstAddr, void *srcAddr, unsigned int size);

/**
 * @brief Compares memory contents between Flash and RAM.
 * @param dstAddr Destination address (typically Flash)
 * @param srcAddr Source address (typically RAM)
 * @param size Number of bytes to compare
 * @return CMD_SUCCESS if equal, COMPARE_ERROR if data differs
 */
unsigned int FLASH_VerifyData(void *dstAddr, void *srcAddr, unsigned int size);

#endif /* _FLASH_H_ */

/**
* @}
*/
/**
* @}
*/
