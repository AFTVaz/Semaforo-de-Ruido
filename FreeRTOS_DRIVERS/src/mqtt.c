/*
 * mqtt.c
 *
 *  Created on: 13/05/2025
 *      Author: André Vaz (48209) e Filipe Cruz (43468)
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include "FreeRTOS.h"
#include "MQTTPacket.h"
#include "transport.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "espserial.h"
#include "wifi.h"
#include "mqtt.h"

static TaskHandle_t xMQTTTaskHandle = NULL;
static QueueHandle_t xMQTTQueue = NULL;
static SemaphoreHandle_t xMQTTMutex = NULL;

static MQTT_StatusValueType xMQTTStatus = MQTT_IDLE;
static MQTT_StateValueType xMQTT_State = MQTT_STATE_INIT;
static MQTT_ConfigCallback xMQTT_ConfigCallback = NULL;

static unsigned char xMQTT_Buffer[MQTT_BUFFER_SIZE];
static MQTT_Item info = {
		.mqtt_payload = xMQTT_Buffer,
		.mqtt_available_length = 0
};

static void MQTT_SetStatus( MQTT_StatusValueType status ) {
	if (xMQTTMutex != NULL) {
		if (xSemaphoreTake(xMQTTMutex, WIFI_SET_STATE_TIMEOUT) == pdTRUE) {
			xMQTTStatus = status;
			xSemaphoreGive(xMQTTMutex);
		}
	}
}

static void MQTT_SetState(MQTT_StateValueType state) {
	if (xMQTTMutex != NULL) {
		if (xSemaphoreTake(xMQTTMutex, WIFI_SET_STATE_TIMEOUT) == pdTRUE) {
			xMQTT_State = state;
			xSemaphoreGive(xMQTTMutex);
		}
	}
}

static bool MQTT_ParsePacket( const char *buffer, void *user_data ){
	if (buffer == NULL || user_data == NULL) return false;

	const char *ipd = strstr(buffer, "+IPD,");
	if (ipd) {
		int link_id = -1;
		int data_len = 0;
		const char *header_end = strchr(ipd, ':');

		if (header_end){
			int fields = sscanf(ipd, "+IPD,%d,%d:", &link_id, &data_len);
			if (fields == 1) {
				data_len = link_id;
				link_id = -1;
			} else if (fields == 2) {
				if(link_id != MQTT_SERVICE_ID) return false;
			} else return false;

			char data[data_len];

			int len = ESPSERIAL_Recv((unsigned char *)&data, data_len);
			if (len == data_len){
				MQTT_Item *info = (MQTT_Item *)user_data;
				memcpy(info->mqtt_payload, data, data_len);
				info->mqtt_available_length = data_len;

				return true;
			}
		}
	}
	return false;
}

static void MQTT_UpdateAttributes(const char* payload, int len) {
	if (xMQTT_ConfigCallback) {
		xMQTT_ConfigCallback(payload, len);
	}
}

static int MQTT_Send( unsigned char *buffer, unsigned int len ){

	if (buffer == NULL || len == 0) return MQTT_ERROR;

	WIFI_ResultValueType result;
	char cmd[128];
	int cipmux = WIFI_GetCIPMUX();

	if (cipmux == 0) {
		snprintf( cmd, sizeof(cmd), "AT+CIPSEND=%d\r\n", len );
	} else {
		snprintf( cmd, sizeof(cmd), "AT+CIPSEND=%d,%d\r\n", MQTT_SERVICE_ID, len );
	}

	if(WIFI_RequestSendPacket( cmd, buffer, len, MQTT_CMD_TIMEOUT )){
		MQTT_SetStatus(MQTT_PROCESSING);
		while((result = WIFI_GetResult()) == WIFI_RESULT_NONE) taskYIELD();
		if(result == WIFI_RESULT_SUCCESS) return len;
	}

	MQTT_SetStatus(MQTT_IDLE);
	return MQTT_ERROR;
}

static int MQTT_Recv( unsigned char *buffer, unsigned int maxlen ) {

	if (buffer == NULL || maxlen == 0) return 0;

	WIFI_ResultValueType result;
	int total_cpy = 0;

	if( info.mqtt_available_length == 0 ){
		if(!WIFI_RequestReceive(MQTT_CMD_TIMEOUT, MQTT_ParsePacket, &info)){
			MQTT_SetStatus(MQTT_IDLE);
			return 0;
		}
		MQTT_SetStatus(MQTT_IDLE);
		while((result = WIFI_GetResult()) == WIFI_RESULT_NONE) taskYIELD();
		if(result == WIFI_RESULT_ERROR){
			return 0;
		}
	}

	total_cpy = (info.mqtt_available_length < maxlen) ? info.mqtt_available_length : maxlen;
	memcpy(buffer, info.mqtt_payload, total_cpy);
	memmove(info.mqtt_payload, info.mqtt_payload + total_cpy, info.mqtt_available_length - total_cpy);
	info.mqtt_available_length -= total_cpy;

	return total_cpy;
}

static bool MQTT_Connect( const char *host, const unsigned short int port,
		const unsigned short int keepalive ){
	WIFI_ResultValueType result;
	char cmd[128];

	int cipmux = WIFI_GetCIPMUX();

	if (cipmux == 0) {
		snprintf(cmd, sizeof(cmd), "AT+CIPSTART=\"TCP\",\"%s\",%d,%d\r\n", host, port, keepalive);
	} else {
		snprintf(cmd, sizeof(cmd), "AT+CIPSTART=%d,\"TCP\",\"%s\",%d,%d\r\n", MQTT_SERVICE_ID, host, port, keepalive);
	}
	if(!WIFI_RequestExpected(cmd, "CONNECT", WIFI_CONN_SUCESS_TIMEOUT)) return false;
	while ((result = WIFI_GetResult()) == WIFI_RESULT_NONE) taskYIELD();
	return result == WIFI_RESULT_SUCCESS;
}

static bool MQTT_Disconnect(void) {
	WIFI_ResultValueType result;
	char cmd[32];

	int cipmux = WIFI_GetCIPMUX();
	if (cipmux == 0)
		snprintf(cmd, sizeof(cmd), "AT+CIPCLOSE\r\n");
	else
		snprintf(cmd, sizeof(cmd), "AT+CIPCLOSE=%d\r\n", MQTT_SERVICE_ID);

	while (!WIFI_RequestExpected(cmd, "OK\r\n", MQTT_CIPCLOSE_TIMEOUT))
		vTaskDelay(MQTT_WAIT_DELAY);

	while ((result = WIFI_GetResult()) == WIFI_RESULT_NONE)
		taskYIELD();

	return result == WIFI_RESULT_SUCCESS;
}

static void MQTT_Task( void *pvParameters ){

	unsigned char buffer[256];
	MQTTTransport transporter;
	MQTTString topic = MQTTString_initializer;
	int result;
	int length;

	TickType_t xQueueWaitTime = xMQTT_ConfigCallback ? pdMS_TO_TICKS(100) : portMAX_DELAY;

	// ESP8266 Transport Layer
	static transport_iofunctions_t iof = { MQTT_Send, MQTT_Recv };
	int transport_socket = transport_open(&iof);

	while (true) {

		if( WIFI_GetState() != WIFI_STATE_CONNECTED){
			MQTT_SetState(MQTT_STATE_INIT);
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		}

		switch (xMQTT_State) {
			case MQTT_STATE_INIT:
				MQTT_SetStatus(MQTT_IDLE);
				if ( MQTT_Connect(MQTT_ADDRESS, MQTT_PORT, CONNECTION_KEEPALIVE_S * 2) ){
					MQTT_SetState(MQTT_STATE_CONNECT);
				}
				break;

			case MQTT_STATE_CONNECT:
				transporter.sck = &transport_socket;
				transporter.getfn = transport_getdatanb;
				transporter.state = 0;

				MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
				connectData.MQTTVersion = 3;
				connectData.username.cstring = MQTT_DEVICE_TOKEN;
				connectData.clientID.cstring = MQTT_DEVICE_TOKEN;
				connectData.keepAliveInterval = CONNECTION_KEEPALIVE_S;
				length = MQTTSerialize_connect(buffer, sizeof(buffer), &connectData);
				// Send CONNECT to the mqtt broker.
				if ((result = transport_sendPacketBuffer(transport_socket, buffer, length)) == length){
					MQTT_SetState(MQTT_STATE_WAIT_CONNECT);
				} else {
					MQTT_SetState(MQTT_STATE_INIT);
				}
				break;

			case MQTT_STATE_WAIT_CONNECT:
				// Wait for CONNACK response from the MQTT broker.
				while (true) {
					if ((result = MQTTPacket_readnb(buffer, sizeof(buffer), &transporter)) == CONNACK) {
						// Check if the connection was accepted.
						unsigned char sessionPresent, connack_rc;
						if ((MQTTDeserialize_connack(&sessionPresent, &connack_rc, buffer,
								sizeof(buffer)) != 1) || (connack_rc != 0)) {
							MQTT_SetState(MQTT_STATE_INIT);
							break;
						} else {
							if(xMQTT_ConfigCallback) MQTT_SetState(MQTT_STATE_SUBSCRIBE);
							else MQTT_SetState(MQTT_STATE_PUBLISH);
							break;
						}
					} else if (result == MQTTPACKET_READ_ERROR) {
						MQTT_SetState(MQTT_STATE_INIT);
						break;
					}
				}
				break;

            case MQTT_STATE_SUBSCRIBE:
            	topic.cstring = "v1/devices/me/attributes";

            	unsigned char dup = 0;   		// sem duplicação
            	unsigned short packetId = 1;   	// identificador do pacote
            	int count = 1;					// número de tópicos
            	int req_qos[1] = {1};			// array de QoS por tópico

            	int len = MQTTSerialize_subscribe(buffer, sizeof(buffer), dup, packetId, count, &topic, req_qos);

            	if (transport_sendPacketBuffer(transport_socket, buffer, len) == len) {
            		MQTT_SetState(MQTT_STATE_WAIT_SUBACK);
            	} else {
            		MQTT_SetState(MQTT_STATE_INIT);
            	}
            	break;

            case MQTT_STATE_WAIT_SUBACK:
            	while (true) {
            		if((result = MQTTPacket_readnb(buffer, sizeof(buffer), &transporter)) == SUBACK){

            			unsigned short returnedPacketId;
            			const int maxGrantedQoS = 1;
            			int countGranted;
            			int grantedQoSs[1];

            			if ((MQTTDeserialize_suback(&returnedPacketId, maxGrantedQoS, &countGranted, grantedQoSs, buffer, sizeof(buffer)) != 1)
            					|| (returnedPacketId != packetId)
								|| (countGranted != maxGrantedQoS)
								|| (grantedQoSs[0] == MQTT_SUBACK_REJECTED)) {
            				MQTT_SetState(MQTT_STATE_INIT);
            				break;
            			}

            			MQTT_SetState(MQTT_STATE_PUBLISH);
            			break;
            		} else if (result == MQTTPACKET_READ_ERROR) {
            			MQTT_SetState(MQTT_STATE_INIT);
            			break;
            		}
            	}
            	break;

            case MQTT_STATE_PUBLISH:

            	char *payload = NULL;

            	if (xQueueReceive(xMQTTQueue, &payload, xQueueWaitTime)) {
            		MQTTString topic = MQTTString_initializer;
            		topic.cstring = "v1/devices/me/telemetry";

            		unsigned char dup = 0;
            		int qos = 0;
            		int retained = 0;
            		unsigned short packetId = 0;

            		int length = strlen(payload);
            		length = MQTTSerialize_publish(buffer, sizeof(buffer), dup, qos, retained,
            				packetId, topic, (unsigned char *)payload, length);

            		if ((result = transport_sendPacketBuffer(transport_socket, buffer, length)) != length) {
            			MQTT_Disconnect();
            			MQTT_SetState(MQTT_STATE_INIT);
            			break;
            		}
            	}

            	if (xMQTT_ConfigCallback){
            		result = MQTTPacket_readnb(buffer, sizeof(buffer), &transporter);
            		if (result == PUBLISH) {
            			unsigned char dup, retained;
            			int qos;
            			unsigned short packet_id;
            			unsigned char *payload_in;
            			int payload_len;
            			MQTTString received_topic;

            			if (MQTTDeserialize_publish(&dup, &qos, &retained,
            					&packet_id, &received_topic, &payload_in,
								&payload_len, buffer, sizeof(buffer))) {

            				if (strstr(received_topic.lenstring.data, "attributes")) {
            					MQTT_UpdateAttributes((const char *)payload_in, payload_len);
            				}
            			}
            		} else if (result == MQTTPACKET_READ_ERROR) {
            			MQTT_Disconnect();
            			MQTT_SetState(MQTT_STATE_INIT);
            			break;
            		}
            	}

            	break;

			default:
				MQTT_SetState(MQTT_STATE_INIT);
				break;
		}
	}
}

void MQTT_Init( MQTT_ConfigCallback callback ) {

	if (callback) {
		xMQTT_ConfigCallback = callback;
	}

	xMQTTMutex = xSemaphoreCreateMutex();
	xMQTTQueue = xQueueCreate(10, sizeof(char *));
	if ( xMQTTMutex != NULL && xMQTTQueue != NULL ){
		xTaskCreate(MQTT_Task, "MQTT_Task", 8 * configMINIMAL_STACK_SIZE, NULL, 1, &xMQTTTaskHandle);
	}
}

bool MQTT_Ready( void ){
	return xMQTTTaskHandle != NULL;
}

bool MQTT_isReadyToPublish( void ){
	return MQTT_GetState() == MQTT_STATE_PUBLISH;
}

void MQTT_Publish(const char *payload) {
	if (!MQTT_isReadyToPublish()) return;

	if (xMQTTQueue != NULL) {

		char fixed_copy[MQTT_PAYLOAD_SIZE];
		strncpy(fixed_copy, payload, sizeof(fixed_copy) - 1);
		fixed_copy[MQTT_PAYLOAD_SIZE - 1] = '\0';

		char *copy = fixed_copy;
		xQueueSend(xMQTTQueue, &copy, 0);
	}
}

void MQTT_NoisePublish( float noiseValue, time_t time ){
	char payload[MQTT_PAYLOAD_SIZE];

	snprintf(payload, sizeof(payload),
			"{\"noise\": %.2f, \"timestamp\": %ld}",
			noiseValue, (long)time);

	MQTT_Publish(payload);
}

MQTT_StatusValueType MQTT_GetStatus( void ){
	MQTT_StatusValueType status = MQTT_IDLE;
	if(xMQTTMutex != NULL){
		if(xSemaphoreTake(xMQTTMutex, WIFI_GET_STATE_TIMEOUT) == pdTRUE){
			status = xMQTTStatus;
			xSemaphoreGive(xMQTTMutex);
		}
	}
	return status;
}

MQTT_StateValueType MQTT_GetState( void ){
	MQTT_StateValueType state = MQTT_STATE_INIT;
	if (xMQTTMutex != NULL){
		if (xSemaphoreTake(xMQTTMutex, WIFI_GET_STATE_TIMEOUT) == pdTRUE){
			state = xMQTT_State;
			xSemaphoreGive(xMQTTMutex);
		}
	}
	return state;
}

void MQTT_Resume( void ) {
	if (xMQTTTaskHandle) {
		xTaskNotifyGive(xMQTTTaskHandle);
	}
}
