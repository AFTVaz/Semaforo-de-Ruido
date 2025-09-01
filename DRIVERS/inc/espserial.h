/**
* @file		espserial.h
* @brief	This header file provides the API for communicating with the ESP8266
*			module over a serial interface. The functions in this API allow for
*			initializing the serial communication, sending commands, and receiving
*			responses from the ESP8266.
* @version	1.0
* @date		17 Mar 2017
* @author	PSampaio
*
* Copyright(C) 2015-2025, PSampaio
* All rights reserved.
*
***********************************************************************
* The software provided in this file is intended for illustrative purposes only.
* It provides an interface for serial communication with the ESP8266 Wi-Fi
* module. The software is supplied "AS IS," without any warranties or guarantees
* of any kind.
**********************************************************************/

#ifndef _ESPSERIAL_H_
#define _ESPSERIAL_H_

/**
 * @addtogroup DRIVERS
 * @{
 */

/**
 * @defgroup ESPSERIAL ESP8266 serial interface
 * @brief This module provides an interface for communication with the ESP8266
 *        Wi-Fi module using a serial communication protocol (UART).
 *        It use the UART3 on LPC1769 (P0.0 and P0.1).
 *        Reset should be connect to P0.5.
 *        It allows the user to initialize the serial interface, send commands
 *        to the module, and receive responses.
 * @{
 */

/**
 * @brief  Initializes the serial communication interface for the ESP8266 module.
 * @param  baudrate The baud rate for serial communication. This sets the speed
 *                  at which data is transmitted between the microcontroller
 *                  and the ESP8266 module.
 * @note   The function configures the serial interface with the specified baud rate
 *         to allow for communication with the ESP8266. Ensure that the baud rate
 *         matches the ESP8266's configured baud rate.
 */
void ESPSERIAL_Init(int baudrate);

/**
 * @brief  Sends data (commands) to the ESP8266 over the serial interface.
 * @param  command A pointer to the buffer containing the data to be sent to the
 *                 ESP8266.
 * @param  size The size of the data buffer in bytes, indicating how much data
 *              to send.
 * @return The number of bytes successfully written to the ESP8266. This is the
 *         number of bytes transmitted from the buffer to the module.
 */
int ESPSERIAL_Send(void *command, int size);

/**
 * @brief  Receives data (response) from the ESP8266 over the serial interface.
 * @param  response A pointer to the buffer where the received data will be
 *                  stored.
 * @param  maxSize The maximum size of the buffer. This limits how many bytes
 *                 the function can receive from the ESP8266.
 * @return The number of bytes successfully read from the ESP8266. This is the
 *         amount of data received and stored in the `response` buffer.
 */
int ESPSERIAL_Recv(unsigned char *response, int maxSize);

/**
 * @}
 */

/**
 * @}
 */

#endif /* _ESPSERIAL_H_ */
