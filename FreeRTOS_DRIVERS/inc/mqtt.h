/**
* @file mqtt.h
* @brief Contains the MQTT (Message Queue Telemetry Transport) API.
* @version 1.0
* @date 13 Mai 2025
* @author André Vaz (48209) e Filipe Cruz (43468)
*
* Note: This file was made based on the file "foopublish.c"
* 		from author psampaio
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

/** @defgroup MQTT MQTT
* @brief Provides the core capabilities for MQTT related functions.
* @{
*/

#ifndef _MQTT_H_
#define _MQTT_H_

#include <time.h>

#define MQTT_SERVICE_ID			1

#define MQTT_ADDRESS			"iot-ps.ddns.net"
#define MQTT_PORT				1884
#define MQTT_DEVICE				"DevGrupo12"
#define MQTT_DEVICE_TOKEN		"SEIOT-G12"

#define MQTT_USER				"seiot-g12@iot-ps.ddns.net"
#define MQTT_PASSWORD			"12345678"

#define MQTT_ERROR				-1

#define CONNECTION_KEEPALIVE_S 	60UL
#define MQTT_QUEUE_TIMEOUT		pdMS_TO_TICKS(100)
#define MQTT_CMD_TIMEOUT       	pdMS_TO_TICKS(5000)
#define MQTT_CIPSTART_TIMEOUT	pdMS_TO_TICKS(5000)
#define MQTT_CIPCLOSE_TIMEOUT	pdMS_TO_TICKS(5000)

#define MQTT_BUFFER_SIZE		512
#define MQTT_PAYLOAD_SIZE		128

#define MQTT_SUBACK_QOS_0		0x00  /* Subscrição aceita com QoS 0 */
#define MQTT_SUBACK_QOS_1		0x01  /* Subscrição aceita com QoS 1 */
#define MQTT_SUBACK_QOS_2		0x02  /* Subscrição aceita com QoS 2 */
#define MQTT_SUBACK_REJECTED	0x80  /* Subscrição recusada pelo broker */

#define MQTT_WAIT_DELAY			pdMS_TO_TICKS(10)

/**
 * @brief Callback function type for processing attributes received via MQTT.
 *
 * 		  This function is called when a message is received on the topic
 * 		  "v1/devices/me/attributes", allowing the user to handle attribute updates
 * 		  sent by the server (ThingsBoard).
 *
 * @param payload Pointer to the received data buffer.
 * @param len Length (in bytes) of the content available in payload.
 */
typedef void (*MQTT_ConfigCallback)(const char* payload, int len);

/**
 * @enum MQTT_StateValueType
 * @brief Represents the internal state of the MQTT client.
 *
 * 		  This enumeration defines the states used in the MQTT task's state machine,
 * 		  guiding the process from connection initialization to publishing and
 * 		  handling attribute updates.
 */
typedef enum MQTT_StateValueType {
    MQTT_STATE_INIT,          /**< Initial state, waiting for Wi-Fi connection or ready to connect to broker */
    MQTT_STATE_CONNECT,       /**< Attempting to send MQTT CONNECT packet */
    MQTT_STATE_WAIT_CONNECT,  /**< Waiting for CONNACK response from the MQTT broker */
    MQTT_STATE_SUBSCRIBE,     /**< Sending SUBSCRIBE packet to subscribe to desired topics */
    MQTT_STATE_WAIT_SUBACK,   /**< Waiting for SUBACK response from the MQTT broker */
    MQTT_STATE_PUBLISH        /**< Ready to publish data to the MQTT broker and handle incoming attribute updates */
} MQTT_StateValueType;

/**
 * @struct MQTT_Item
 * @brief Structure for storing MQTT payload data.
 *
 * Used to transfer received MQTT data from the Wi-Fi module to the application.
 */
typedef struct {
	unsigned char *mqtt_payload; 			/**< Pointer to the buffer containing MQTT payload data */
	unsigned int mqtt_available_length;		/**< Length of the received MQTT payload */
} MQTT_Item;

/**
 * @enum MQTT_StatusValueType
 * @brief Internal status of the MQTT client.
 */
typedef enum MQTT_StatusValueType {
	MQTT_IDLE,        /**< MQTT client is idle or ready for a new request */
	MQTT_PROCESSING  /**< MQTT client is processing a mqtt request */
} MQTT_StatusValueType;

/**
 * @brief Initializes the MQTT task.
 *
 *        Creates a FreeRTOS mutex, queue, and task for managing MQTT operations.
 *        The MQTT task handles connection to the broker, publishing messages,
 *        and session maintenance according to the MQTT protocol.
 *
 * @note This function should be called after the Wi-Fi system is initialized,
 *       as it depends on an active TCP connection managed by the Wi-Fi driver.
 */
void MQTT_Init( MQTT_ConfigCallback callback );

/**
 * @brief Checks whether the MQTT task is ready.
 *
 *        Indicates whether the MQTT task has been successfully created and is running.
 *
 * @return true if MQTT task handle is valid; false otherwise.
 */
bool MQTT_Ready( void );

/**
 * @brief Checks whether the MQTT task is ready to publish a message.
 *
 * 		  This function returns true if the MQTT task is in the PUBLISH state,
 * 		  indicating that it is ready to send a new message to the MQTT broker.
 *
 * @return true if ready to publish; false otherwise.
 */
bool MQTT_isReadyToPublish(void);

/**
 * @brief Publishes a pre-formatted JSON payload to the MQTT broker.
 *
 * 		  This function sends a JSON-formatted string to the MQTT transmission queue
 * 		  if the system is in a state ready to publish. The message will be processed
 * 		  and transmitted by the MQTT task.
 *
 * @param payload The JSON-formatted string to publish.
 */
void MQTT_Publish(const char *payload);

/**
 * @brief Publishes noise measurement data to the MQTT broker.
 *
 *        Formats the noise value and timestamp into a JSON payload and
 *        sends it using MQTT_Publish.
 *
 * @param noiseValue The noise measurement as a float.
 * @param time The timestamp associated with the measurement.
 */
void MQTT_NoisePublish(float noiseValue, time_t time);

/**
 * @brief Retrieves the current status of the MQTT client.
 *
 *        Returns the internal status of the MQTT client in a thread-safe manner
 *        by using a FreeRTOS mutex.
 *
 * @return The current MQTT client status as a value of MQTT_StatusValueType.
 *
 * @retval MQTT_IDLE If the client is idle or ready for a new request.
 * @retval MQTT_PROCESSING If the client is currently processing an MQTT request.
 */
MQTT_StatusValueType MQTT_GetStatus(void);

/**
 * @brief Retrieves the current internal state of the MQTT client state machine.
 *
 *        Returns the current operational state of the MQTT client, indicating
 *        whether it is initializing, attempting to connect, waiting for a response,
 *        or ready to publish. This function is thread-safe and uses a FreeRTOS mutex
 *        to ensure consistent access.
 *
 * @return The current state of the MQTT client as a value of MQTT_StateValueType.
 *
 * @retval MQTT_STATE_INIT Initial state, waiting for Wi-Fi or ready to initiate connection.
 * @retval MQTT_STATE_CONNECT Attempting to send an MQTT CONNECT packet.
 * @retval MQTT_STATE_WAIT_CONNECT Waiting for CONNACK response from the broker.
 * @retval MQTT_STATE_PUBLISH Ready to publish data to the MQTT broker.
 */
MQTT_StateValueType MQTT_GetState(void);

/**
 * @brief Resume the MQTT task after Wi-Fi connection is established.
 *
 * 		  This function notifies the MQTT task to resume its operation
 * 		  after a successful Wi-Fi connection and configuration.
 * 		  By resuming the MQTT task, this function allows the MQTT client to proceed with
 * 		  connecting to the broker, subscribing to topics, and publishing messages.
 *
 * @note If the MQTT task handle is not valid this function has no effect.
 */
void MQTT_Resume( void );

#endif /* _MQTT_H_ */

/**
* @}
*/
/**
* @}
*/
