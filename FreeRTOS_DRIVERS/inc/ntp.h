/**
* @file ntp.h
* @brief Contains the NTP (Network Time Protocol) API.
* @version 1.0
* @date 05 Mai 2025
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

/** @defgroup NTP NTP
* @brief Provides the core capabilities for NTP related functions.
* @{
*/

#ifndef _NTP_H_
#define _NTP_H_

#include <time.h>

#define NTP_SERVICE_ID			0

#define NTP_CONN_TYPE			"UDP"
#define NTP_SERVER				"pool.ntp.org"
#define NTP_REMOTE_PORT			123
#define NTP_PACKET_SIZE			48
#define NTP_TRANSMIT_S	 		40
#define NTP_TRANSMIT_F		 	44
#define UNIX_EPOCH 				2208988800UL

#define NTP_CIPSTART_TIMEOUT	pdMS_TO_TICKS(5000)
#define NTP_CIPCLOSE_TIMEOUT	pdMS_TO_TICKS(5000)
#define NTP_TRANSACTION_TIMEOUT	pdMS_TO_TICKS(5000)
#define NTP_CONN_TIMEOUT		pdMS_TO_TICKS(10000)

#define NTP_RECV_DELAY			pdMS_TO_TICKS(5)
#define NTP_COMMAND_DELAY		pdMS_TO_TICKS(100)
#define NTP_WAIT_DELAY			pdMS_TO_TICKS(10)

/**
 * @struct NTP_Packet
 * @brief Represents a 48-byte NTP (Network Time Protocol) packet structure.
 *
 * This structure defines the standard format of an NTP message used for
 * time synchronization between clients and servers.
 */
typedef struct NTP_Packet{
    uint8_t li_vn_mode;			// Eight bits. li, vn, and mode.
								// li.      Two bits. Leap indicator.
								// vn.      Three bits. Version number of the protocol.
								// mode.    Three bits. Client will pick mode 3 for client.

    uint8_t stratum;			// Eight bits. Stratum level of the local clock.
    uint8_t poll;				// Eight bits. Maximum interval between successive messages.
    uint8_t precision;			// Eight bits. Precision of the local clock.

    uint32_t rootDelay;			// 32 bits. Total round trip delay time.
    uint32_t rootDispersion;	// 32 bits. Max error aloud from primary clock source.
    uint32_t refId;				// 32 bits. Reference clock identifier.

    uint32_t refTm_s;			// 32 bits. Reference time-stamp seconds.
    uint32_t refTm_f;			// 32 bits. Reference time-stamp fraction of a second.

    uint32_t origTm_s;			// 32 bits. Originate time-stamp seconds.
    uint32_t origTm_f;			// 32 bits. Originate time-stamp fraction of a second.

    uint32_t rxTm_s;			// 32 bits. Received time-stamp seconds.
    uint32_t rxTm_f;			// 32 bits. Received time-stamp fraction of a second.

    uint32_t txTm_s;			// 32 bits. Transmit time-stamp seconds. Important
    uint32_t txTm_f;			// 32 bits. Transmit time-stamp fraction of a second.
} NTP_Packet;					// Total: 384 bits or 48 bytes.


/**
 * @enum NTP_StateValueType
 * @brief Internal state of the NTP client.
 */
typedef enum NTP_StateValueType {
    NTP_IDLE,        /**< NTP client is idle or ready for a new request */
    NTP_PROCESSING,  /**< NTP client is processing a time request */
    NTP_READY        /**< NTP client has received a valid timestamp */
} NTP_StateValueType;

/**
 * @brief Initializes the NTP module and creates the background task.
 *
 *        This function sets up the mutex used to protect access to shared NTP state
 *        and creates the FreeRTOS task responsible for handling NTP time synchronisation.
 *
 * @note Must be called once before any other NTP-related function.
 */
void NTP_Init(void);

/**
 * @brief Checks if the NTP module has been initialised.
 *
 *        This function verifies whether the NTP background task has been successfully created,
 *        indicating that the NTP module is ready to process time requests.
 *
 * @retval true  If the NTP task has been created and the module is ready.
 * @retval false If the NTP task has not been created (NTP_Init was not called or failed).
 */
bool NTP_Ready( void );

/**
 * @brief Retrieves the current state of the NTP client.
 *
 *        Returns the internally tracked NTP client state, ensuring thread safety
 *        by using a FreeRTOS mutex.
 *
 * @return The current state as a value of NTP_StateValueType.
 *
 * @retval NTP_IDLE If the client is idle or ready for a new request.
 * @retval NTP_PROCESSING If the client is processing a time request.
 * @retval NTP_READY If the client has received a valid timestamp.
 */
NTP_StateValueType NTP_GetState(void);

/**
 * @brief Sends a time request to the configured NTP server.
 *
 *        This function notifies the NTP task to start the process of connecting,
 *        sending the NTP packet, and waiting for a valid response.
 *
 * @note This function is non-blocking needs a follow up call of NTP_GetTime.
 * 		 Use NTP_RequestAndWait to block until time is available.
 */
void NTP_RequestTime( void );

/**
 * @brief Retrieves the latest timestamp received from the NTP server.
 *
 *        This function returns the converted "struct tm" (UTC/GMT) if a valid
 *        timestamp is available. It resets the internal state to "NTP_IDLE"
 *        after a successful retrieval.
 *
 * @param timestamp Pointer to a "time_t" where the result will be stored.
 *
 * @retval true  If a valid timestamp was available and "timestamp" was populated.
 * @retval false If the timestamp is not available or "timestamp" is NULL.
 *
 * @note This function is non-blocking and should be used after calling NTP_RequestTime.
 *       Use NTP_RequestAndWait for a blocking alternative.
 */
bool NTP_GetTimestamp( time_t *timestamp );

/**
 * @brief Sends an NTP request and blocks until a response is received.
 *
 *        This function combines the request and polling logic to retrieve
 *        the timestamp in a blocking manner, useful in cases where the application
 *        flow depends on synchronized time.
 *
 * @param timestamp Pointer to a "time_t" where the result will be stored.
 *
 * @retval true  If the timestamp was received and "timestamp" was successfully populated.
 * @retval false If an error or if "timestamp" is NULL.
 */
bool NTP_RequestAndWait( time_t *timestamp );

#endif /* _NTP_H_ */

/**
* @}
*/
/**
* @}
*/
