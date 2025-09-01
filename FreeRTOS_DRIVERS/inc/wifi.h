/**
* @file wifi.h
* @brief Contains the Wi-Fi API.
* @version 1.0
* @date 04 Mai 2025
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

/** @defgroup WIFI WIFI
* @brief Provides the core capabilities for Wi-Fi related functions.
* @{
*/

#ifndef _WIFI_H_
#define _WIFI_H_

#include "ntp.h"
#include "mqtt.h"

#define ESP01_DEFAULT_BAUDRATE		115200

#define WIFI_MAX_IPv4_LENGTH 		16
#define WIFI_IPv4_DEFAULT			"0.0.0.0"
#define WIFI_MAX_MAC_LENGTH 		18
#define WIFI_MAC_DEFAULT			"00.00.00.00.00.00"

#define WIFI_MAX_BUFFER 			512

#define WIFI_SET_STATE_TIMEOUT		pdMS_TO_TICKS(100)
#define WIFI_GET_STATE_TIMEOUT		pdMS_TO_TICKS(100)
#define WIFI_GET_IP_TIMEOUT			pdMS_TO_TICKS(5000)
#define WIFI_GET_MAC_TIMEOUT		pdMS_TO_TICKS(5000)
#define WIFI_AT_TIMEOUT				pdMS_TO_TICKS(5000)
#define WIFI_CWMODE_TIMEOUT			pdMS_TO_TICKS(5000)
#define WIFI_CIPMUX_TIMEOUT			pdMS_TO_TICKS(5000)
#define WIFI_DCONN_TIMEOUT			pdMS_TO_TICKS(5000)
#define WIFI_CONN_SUCESS_TIMEOUT	pdMS_TO_TICKS(50000)

#define WIFI_COMMAND_DELAY			pdMS_TO_TICKS(5)
#define WIFI_WAIT_DELAY				pdMS_TO_TICKS(10)
#define WIFI_SEND_DELAY				pdMS_TO_TICKS(500)
#define WIFI_CLEAN_DELAY			pdMS_TO_TICKS(5000)

/**
 * @typedef WIFI_ParserFunc
 * @brief Function pointer for parser callbacks.
 *
 * @param buffer The input buffer to parse.
 * @param user_data Pointer to user data or storage.
 *
 * @retval true if expected data is found or parsed.
 * @retval false otherwise.
 */
typedef bool (*WIFI_ParserFunc)( const char *buffer, void *user_data );

/**
 * @enum WIFI_StateValueType
 * @brief Enumeration of possible Wi-Fi states.
 */
typedef enum WIFI_StateValueType {
    WIFI_STATE_DISCONNECTED,  /**< Not connected to a Wi-Fi network */
    WIFI_STATE_CONNECTED,     /**< Successfully connected to a Wi-Fi network */
    WIFI_STATE_ERROR          /**< Error occurred during Wi-Fi operations */
} WIFI_StateValueType;

/**
 * @enum WIFI_ResultValueType
 * @brief Result of the last Wi-Fi command executed.
 */
typedef enum WIFI_ResultValueType {
    WIFI_RESULT_NONE,     /**< No result available */
    WIFI_RESULT_SUCCESS,  /**< Command executed successfully */
    WIFI_RESULT_ERROR     /**< Command execution failed */
} WIFI_ResultValueType;

/**
 * @enum WIFI_CommandValueType
 * @brief Enumeration of commands for Wi-Fi task operations.
 */
typedef enum {
    WIFI_CMD_DISCONNECT,  	/**< Disconnect from Wi-Fi network */
    WIFI_CMD_CONNECT,     	/**< Connect to Wi-Fi network */
    WIFI_CMD_GET_INFO,     	/**< Request the IP and MAC address */
    WIFI_CMD_GET_EXPECTED, 	/**< Send command and wait for expected response */
	WIFI_CMD_SEND,          /**< Send command or data packet */
	WIFI_CMD_RECEIVE,       /**< Receive a data packet */
	WIFI_CMD_TRANSACTION	/**< Send and then receive as a single transaction. */
} WIFI_CommandValueType;

/**
 * @enum WIFI_ServiceValueType
 * @brief Specifies which service(s) to initialize along with Wi-Fi.
 */
typedef enum {
    WIFI_SERVICE_NONE,  /**< Do not initialize any service */
    WIFI_SERVICE_NTP,   /**< Initialize only NTP service */
    WIFI_SERVICE_MQTT,  /**< Initialize only MQTT service */
    WIFI_SERVICE_BOTH   /**< Initialize both NTP and MQTT services */
} WIFI_ServiceValueType;

/**
 * @enum WIFI_ConnectionModeValueType
 * @brief Defines the connection mode for the ESP Wi-Fi module.
 */
typedef enum {
    WIFI_SINGLE_CONNECTION,   /**< Single connection mode (CIPMUX=0) */
    WIFI_MULTIPLE_CONNECTION  /**< Multiple connection mode (CIPMUX=1) */
} WIFI_ConnectionModeValueType;

/**
 * @struct WIFI_Config
 * @brief Configuration structure for initializing Wi-Fi and related services.
 *
 * 		  This structure holds the necessary parameters for Wi-Fi connection
 * 		  and the selection of additional services such as NTP and MQTT.
 */
typedef struct WIFI_Config {
    char *ssid;                       			/**< SSID of the Wi-Fi network to connect */
    char *password;                  			/**< Password for the Wi-Fi network */
    WIFI_ServiceValueType service;   			/**< Service(s) to initialize: NONE, NTP, MQTT, or BOTH */
    MQTT_ConfigCallback xMQTT_ConfigCallback; 	/**< Callback function invoked on MQTT configuration/attribute updates */
} WIFI_Config;

/**
 * @struct WIFI_Info
 * @brief Structure used to hold the IP and MAC address of the Wi-Fi module.
 */
typedef struct WIFI_Info {
    char *ip;   /**< Pointer to the IPv4 address string */
    char *mac;  /**< Pointer to the MAC address string */
} WIFI_Info;

/**
 * @struct WIFI_QueueItem
 * @brief Structure used to send commands via queue to the Wi-Fi task.
 */
typedef struct WIFI_QueueItem{
    WIFI_CommandValueType cmd;	/**< The command to execute */
    int timeoutMS;				/**< Timeout for the command (in milliseconds) */
    char *command;			/**< AT command string */
    uint8_t *packet;		/**< Binary Data Packet */
    void *user_data;			/**< User data or output pointer */
    int length;					/**< Length of data (for send/receive operations) */
    WIFI_ParserFunc parser;		/**< Response parser function */
} WIFI_QueueItem;

/**
 * @brief Initializes the Wi-Fi module and optionally related services.
 *
 *        This function stores the SSID and password used for connection,
 *        creates synchronization primitives, and starts the internal Wi-Fi task.
 *        It can also initialize NTP and/or MQTT services depending on the configuration.
 *
 * @param config Pointer to a WIFI_ConfigType structure containing the SSID, password,
 *               and service initialization options.
 *
 * @note The config pointer and its fields must not be NULL.
 *       SSID and password strings must remain valid during initialization.
 */
void WIFI_Init(WIFI_Config *config);

/**
 * @brief Checks whether the Wi-Fi task is ready.
 *
 *        Indicates whether the Wi-Fi task has been successfully created and is running.
 *
 * @retval true If the Wi-Fi task is active.
 * @retval false If the task is not yet created or has failed.
 */
bool WIFI_Ready( void );

/**
 * @brief Triggers a connection attempt to the configured Wi-Fi network.
 *
 *        This function signals the internal Wi-Fi task to begin the connection
 *        process using the SSID and password previously provided via WIFI_Init.
 *
 */
void WIFI_Connect(void);

/**
 * @brief Disconnects from the current Wi-Fi network.
 *
 * 		  This function sends the appropriate AT command to disconnect the ESP module
 * 		  from the Wi-Fi access point and updates the internal connection state.
 *
 */
void WIFI_Disconnect( void );

/**
 * @brief Requests the current IP and MAC addresses from the module.
 *
 * 		  This function sends a command to the ESP module to retrieve the current assigned IP and MAC addresses.
 * 		  The result will be stored internally and can be retrieved with WIFI_GetSavedIP and WIFI_GetSavedMAC.
 *
 * @note If the MQTT module is currently processing, this request will be ignored.
 *
 * @retval true  If the request was successfully queued.
 * @retval false If the MQTT module is busy or the request could not be enqueued.
 */
bool WIFI_RequestInfo(void);

/**
 * @brief Sends a custom AT command and checks for an expected response.
 *
 * 		  This function queues a custom command to be sent to the Wi-Fi module,
 * 		  and the task will wait for a response matching the given expected string.
 *
 * @param command The null-terminated AT command string to send.
 * @param expected The null-terminated string expected in the response.
 * @param timeout_ms Timeout in milliseconds to wait for the response.
 *
 * @note If the MQTT module is currently processing, this request will be ignored.
 *
 * @retval true  If the command was successfully queued.
 * @retval false If the MQTT module is busy or parameters are invalid.
 */
bool WIFI_RequestExpected( char *command, char *expected, int timeout_ms );

/**
 * @brief Sends a raw data packet to the Wi-Fi module.
 *
 *        This function queues a request to send a buffer of bytes (e.g., MQTT/NTP data)
 *        to the ESP-01 module. The data will be transmitted by the internal Wi-Fi task.
 *
 * @param command The null-terminated AT command string to send.
 * @param data Pointer to the data packet to send.
 * @param length Number of bytes in the data packet.
 * @param timeout_ms Timeout in milliseconds to wait for the response.
 *
 * @retval true If the send request was queued successfully.
 * @retval false If the MQTT layer is busy or the queue is unavailable.
 */
bool WIFI_RequestSendPacket( char *command, uint8_t *data, int length, int timeout_ms );

/**
 * @brief Receives a response from the Wi-Fi module with a custom parser.
 *
 *        This function is intended to be used while MQTT is actively processing.
 *        It enqueues a request to wait for a response from the ESP module and parse it
 *        using the provided parser function.
 *
 * @param timeout_ms Timeout in milliseconds to wait for the response.
 * @param parser_recv The parser function that will process the response.
 * @param uData User data or buffer that the parser will fill.
 *
 * @retval true If the receive request was queued successfully.
 * @retval false If the MQTT is not in processing mode or the queue is unavailable.
 */
bool WIFI_RequestReceive(int timeout_ms, WIFI_ParserFunc parser_recv, void *uData);

/**
 * @brief Performs an atomic transaction (send followed by receive).
 *
 *        This function sends a data packet to the ESP module and waits for a response
 *        in a single queued transaction. It ensures that the send/receive operation is
 *        performed without interleaving.
 *
 * @param command The null-terminated AT command string to send.
 * @param data Pointer to the data to send.
 * @param length Length of the data to send (in bytes).
 * @param timeout_ms Timeout to wait for the response (in milliseconds).
 * @param parser_recv Parser function to process the response.
 * @param uData Pointer to user data or response buffer.
 *
 * @retval true If the transaction request was queued successfully.
 * @retval false If the MQTT is currently busy or the queue is unavailable.
 */
bool WIFI_RequestPacketTransaction( char *command, uint8_t *data, int length, int timeout_ms, WIFI_ParserFunc parser_recv, void *uData );

/**
 * @brief Returns the current CIPMUX (connection mode) setting.
 *
 * 		  This function retrieves the current connection mode used by the Wi-Fi module.
 * 		  It indicates whether the module is configured for single or multiple TCP/UDP connections.
 *
 * @return The current connection mode as a value of WIFI_ConnectionModeValueType.
 *
 * @retval WIFI_SINGLE_CONNECTION If the module is configured for a single connection (CIPMUX=0).
 * @retval WIFI_MULTIPLE_CONNECTION If the module is configured for multiple connections (CIPMUX=1).
 */
WIFI_ConnectionModeValueType WIFI_GetCIPMUX(void);

/**
 * @brief Retrieves the current state of the Wi-Fi connection.
 *
 *        Returns the internally tracked connection state, ensuring thread safety
 *        by using a FreeRTOS mutex.
 *
 * @return The current state as a value of WIFI_StateValueType.
 *
 * @retval WIFI_STATE_DISCONNECTED If the module is not connected.
 * @retval WIFI_STATE_CONNECTED If the module is successfully connected.
 * @retval WIFI_STATE_ERROR If an error occurred during the last operation.
 */
WIFI_StateValueType WIFI_GetState(void);

/**
 * @brief Returns the result of the last Wi-Fi command.
 *
 * 		  This function retrieves the result of the last command handled by the Wi-Fi task.
 *
 * @return The current result as a value of WIFI_ResultValueType.
 *
 * @retval WIFI_RESULT_NONE No result available yet.
 * @retval WIFI_RESULT_SUCCESS The last command completed successfully.
 * @retval WIFI_RESULT_ERROR The last command failed or timed out.
 */
WIFI_ResultValueType WIFI_GetResult(void);

/**
 * @brief Copies the last saved IP address into the provided buffer.
 *
 * 		  This function retrieves the last IP address successfully obtained
 * 		  from the ESP module and copies it to the user-provided buffer.
 *
 * @param buffer Pointer to a character buffer where the IP string will be copied.
 *               Must be at least WIFI_MAX_IPv4_LENGTH bytes in size.
 *
 * @retval true If the IP address was successfully copied.
 * @retval false If the buffer is NULL or the mutex could not be acquired.
 */
bool WIFI_GetSavedIP(char *buffer);

/**
 * @brief Copies the last saved MAC address into the provided buffer.
 *
 * 		  This function retrieves the last MAC address successfully obtained
 * 		  from the ESP module and copies it to the user-provided buffer.
 *
 * @param buffer Pointer to a character buffer where the MAC string will be copied.
 *               Must be at least WIFI_MAX_MAC_LENGTH bytes in size.
 *
 * @retval true If the MAC address was successfully copied.
 * @retval false If the buffer is NULL or the mutex could not be acquired.
 */
bool WIFI_GetSavedMAC(char *buffer);

/**
 * @brief Waits for the WiFi connection process to complete.
 *
 *        This function sends a connection request to the ESP-01 module
 *        (via "WIFI_Connect") and continuously polls the connection state
 *        until a successful connection is established or a timeout occurs.
 *
 * @retval true If the module reached WIFI_STATE_CONNECTED within the timeout.
 * @retval false If the timeout was reached before a successful connection.
 */
bool WIFI_WaitConnect(void);

/**
 * @brief Waits for the WiFi disconnection process to complete.
 *
 *        This function sends a disconnection request to the ESP-01 module
 *        (via "WIFI_Disconnect") and continuously polls the connection state
 *        until the module is fully disconnected or a timeout occurs.
 *
 * @retval true If the module reached WIFI_STATE_DISCONNECTED within the timeout.
 * @retval false If the timeout was reached before the disconnection completed.
 */
bool WIFI_WaitDisconnect(void);


/**
 * @brief Waits for a valid IP address to be acquired.
 *
 *        This function continuously polls the result of a previously requested
 *        IP acquisition (via "WIFI_RequestIP") until a valid IP address is received
 *        or a timeout occurs.
 *
 * @param buffer Pointer to a buffer where the valid IP address will be stored.
 *               Must be at least WIFI_MAX_IPv4_LENGTH bytes.
 *
 * @retval true If a valid IP address was retrieved.
 * @retval false If the timeout was reached or an error occurred.
 */
bool WIFI_WaitIP( char *buffer );

/**
 * @brief Waits for a valid MAC address to be acquired.
 *
 *        This function continuously polls the result of a previously requested
 *        MAC acquisition (via "WIFI_RequestMAC") until a valid MAC address is received
 *        or a timeout occurs.
 *
 * @param buffer Pointer to a buffer where the valid MAC address will be stored.
 *               Must be at least WIFI_MAX_MAC_LENGTH bytes.
 *
 * @retval true If a valid MAC address was retrieved.
 * @retval false If the timeout was reached or an error occurred.
 */
bool WIFI_WaitMAC( char *buffer );

#endif /* _WIFI_H_ */

/**
* @}
*/
/**
* @}
*/
