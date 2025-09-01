/*
 * wifi.c
 *
 *  Created on: 04/05/2025
 *      Author: André Vaz (48209) e Filipe Cruz (43468)
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "wifi.h"
#include "espserial.h"
#include "mqtt.h"
#include "ntp.h"

static char *pcWIFI_ssid = NULL;
static char *pcWIFI_password = NULL;
static char cWIFI_IP[WIFI_MAX_IPv4_LENGTH] = {WIFI_IPv4_DEFAULT};
static char cWIFI_MAC[WIFI_MAX_MAC_LENGTH] = {WIFI_MAC_DEFAULT};
static WIFI_ConnectionModeValueType xWIFI_CIPMUX = WIFI_SINGLE_CONNECTION;

static TaskHandle_t xWIFITaskHandle = NULL;
static SemaphoreHandle_t xWIFIMutex = NULL;
static QueueHandle_t xWIFIQueue = NULL;

static WIFI_StateValueType xWIFI_State = WIFI_STATE_DISCONNECTED;
static WIFI_ResultValueType xWIFI_Result = WIFI_RESULT_NONE;

static WIFI_Info xWIFI_Info = {
		.ip = cWIFI_IP,
		.mac = cWIFI_MAC
};

static bool WIFI_ParseExpected( const char *buffer, void *user_data ){
	const char *expected = (const char *)user_data;
	return strstr(buffer, expected) != NULL;
}

static bool WIFI_ParseInfo(const char *buffer, void *user_data) {
	WIFI_Info *info = (WIFI_Info *)user_data;

	int total_info_cpy = 0;

	const char *tag_ip = "+CIFSR:STAIP,\"";
	const char *tag_mac = "+CIFSR:STAMAC,\"";

	char *start_ip = strstr(buffer, tag_ip);
	if (start_ip) {
		start_ip += strlen(tag_ip);
		char *end_ip = strchr(start_ip, '"');
		if (end_ip && info->ip) {
			size_t ip_len = end_ip - start_ip;
			if (ip_len < WIFI_MAX_IPv4_LENGTH) {
				strncpy(info->ip, start_ip, ip_len);
				info->ip[ip_len] = '\0';
				++total_info_cpy;
			}
		}
	}

	char *start_mac = strstr(buffer, tag_mac);
	if (start_mac) {
		start_mac += strlen(tag_mac);
		char *end_mac = strchr(start_mac, '"');
		if (end_mac && info->mac) {
			size_t mac_len = end_mac - start_mac;
			if (mac_len < WIFI_MAX_MAC_LENGTH) {
				strncpy(info->mac, start_mac, mac_len);
				info->mac[mac_len] = '\0';
				++total_info_cpy;
			}
		}
	}

	return total_info_cpy == 2;
}

static bool WIFI_Send( const void *data, int size ) {
	if (data == NULL || size <= 0) return false;
	if (ESPSERIAL_Send((void *)data, size) != size) return false;
	vTaskDelay(WIFI_SEND_DELAY);
	return true;
}


static bool WIFI_Receive( int timeout_ms, WIFI_ParserFunc parser, void *user_data ) {
	if (parser == NULL ) return false;

	char response[WIFI_MAX_BUFFER];
	int index = 0;
	TickType_t start = xTaskGetTickCount();
	TickType_t timeout_ticks = pdMS_TO_TICKS(timeout_ms);

	while ((xTaskGetTickCount() - start) < timeout_ticks) {
		unsigned char c;
		int len = ESPSERIAL_Recv(&c, 1);
		if (len == 1) {
			if (index < WIFI_MAX_BUFFER - 1) {
				response[index++] = c;
				response[index] = '\0';
			}

			if (parser(response, user_data))
				return true;
		}
		vTaskDelay(WIFI_COMMAND_DELAY);
	}
	return false;
}

static bool WIFI_Command( const char *cmd, WIFI_ParserFunc parser, void *user_data, int timeout_ms ) {
	if (!WIFI_Send( cmd, strlen(cmd))) return false;
	if (!WIFI_Receive(timeout_ms, parser, user_data)) return false;
	return true;
}

static void WIFI_SetState( WIFI_StateValueType state ) {
	if (xWIFIMutex != NULL) {
		if (xSemaphoreTake(xWIFIMutex, WIFI_SET_STATE_TIMEOUT) == pdTRUE) {
			xWIFI_State = state;
			xSemaphoreGive(xWIFIMutex);
		}
	}
}

static void WIFI_SetResult( WIFI_ResultValueType result ){
	if (xWIFIMutex != NULL) {
		if (xSemaphoreTake(xWIFIMutex, WIFI_SET_STATE_TIMEOUT) == pdTRUE) {
			xWIFI_Result = result;
			xSemaphoreGive(xWIFIMutex);
		}
	}
}

static bool WIFI_Connection( void ){
	if (WIFI_GetState() == WIFI_STATE_CONNECTED) return true;

	//    if (!WIFI_Command("AT\r\n", WIFI_ParseExpected, "OK\r\n", WIFI_AT_TIMEOUT)) return false;
	if (!WIFI_Command( "AT+CWMODE=1\r\n", WIFI_ParseExpected, "OK\r\n", WIFI_CWMODE_TIMEOUT )) return false;

	char cmd[128];
	snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"\r\n", pcWIFI_ssid, pcWIFI_password);
	return WIFI_Command( cmd, WIFI_ParseExpected, "OK\r\n", WIFI_CONN_SUCESS_TIMEOUT );
}

static bool WIFI_Disconnection( void ){
	return WIFI_Command( "AT+CWQAP\r\n", WIFI_ParseExpected, "OK\r\n", WIFI_DCONN_TIMEOUT );
}

static void WIFI_Clean( void ){
	WIFI_Receive( WIFI_CLEAN_DELAY, WIFI_ParseExpected, "ready" );
}

static bool WIFI_GetInfo( void ){
	return WIFI_Command( "AT+CIFSR\r\n", WIFI_ParseInfo, &xWIFI_Info, WIFI_GET_IP_TIMEOUT );
}

static bool WIFI_ConfigNumberOfConnections( void ){
	char cmd[15];
	snprintf(cmd, sizeof(cmd), "AT+CIPMUX=%d\r\n", xWIFI_CIPMUX);
	return WIFI_Command( cmd, WIFI_ParseExpected, "OK\r\n", WIFI_CIPMUX_TIMEOUT );
}

static void WIFI_Task( void *pvParameters ){
	ESPSERIAL_Init( ESP01_DEFAULT_BAUDRATE );
	WIFI_Clean();

	WIFI_QueueItem item;

	while ( true ) {
		if (xQueueReceive(xWIFIQueue, &item, portMAX_DELAY)) {
        	switch (item.cmd) {
                case WIFI_CMD_CONNECT:
                	if (WIFI_Connection() && WIFI_GetInfo()){
                		if(strcmp(cWIFI_IP, WIFI_IPv4_DEFAULT) != 0){
                			if(WIFI_ConfigNumberOfConnections()){
                				WIFI_SetState(WIFI_STATE_CONNECTED);
                				break;
                			}
                		}
                	}

                	WIFI_SetState(WIFI_STATE_ERROR);
                	break;

                case WIFI_CMD_DISCONNECT:
                	if (WIFI_GetState() == WIFI_STATE_CONNECTED) {
                		if (WIFI_Disconnection()) {
                			WIFI_SetState(WIFI_STATE_DISCONNECTED);
                		} else {
                			WIFI_SetState(WIFI_STATE_ERROR);
                		}
                	}
                	break;

                case WIFI_CMD_GET_INFO:
                	if (WIFI_GetInfo()) {
                		WIFI_SetResult(WIFI_RESULT_SUCCESS);
                	} else {
                		WIFI_SetResult(WIFI_RESULT_ERROR);
                	}
                	break;

                case WIFI_CMD_GET_EXPECTED:
                	if (WIFI_Command(item.command, WIFI_ParseExpected, item.user_data, item.timeoutMS)) {
                		WIFI_SetResult(WIFI_RESULT_SUCCESS);
                	} else {
                		WIFI_SetResult(WIFI_RESULT_ERROR);
                	}
                	break;

                case WIFI_CMD_SEND:
                	if (WIFI_Command(item.command, WIFI_ParseExpected, ">", item.timeoutMS)) {
                		if (WIFI_Send(item.packet, item.length)) {
                			WIFI_SetResult(WIFI_RESULT_SUCCESS);
                			break;
                		}
                	}
                	WIFI_SetResult(WIFI_RESULT_ERROR);
                	break;

                case WIFI_CMD_RECEIVE:
                	if (WIFI_Receive( item.timeoutMS, item.parser, item.user_data )) {
                		WIFI_SetResult(WIFI_RESULT_SUCCESS);
                	} else {
                		WIFI_SetResult(WIFI_RESULT_ERROR);
                	}
                	break;

                case WIFI_CMD_TRANSACTION:
                	if (WIFI_Command(item.command, WIFI_ParseExpected, ">", item.timeoutMS)) {
                		if (WIFI_Send(item.packet, item.length)) {
                			if (WIFI_Receive(item.timeoutMS, item.parser, item.user_data)) {
                				WIFI_SetResult(WIFI_RESULT_SUCCESS);
                				break;
                			}
                		}
                	}
                	WIFI_SetResult(WIFI_RESULT_ERROR);
                	break;

                default:
                	break;
            }
        }
    }
}

void WIFI_Init(WIFI_Config *config) {
	if (config == NULL || config->ssid == NULL || config->password == NULL) return;
	if (config->service < WIFI_SERVICE_NONE || config->service > WIFI_SERVICE_BOTH) return;

	pcWIFI_ssid = config->ssid;
	pcWIFI_password = config->password;

	xWIFIMutex = xSemaphoreCreateMutex();
	xWIFIQueue = xQueueCreate(10, sizeof(WIFI_QueueItem));

	if (xWIFIMutex != NULL && xWIFIQueue != NULL) {
		xTaskCreate(WIFI_Task, "WiFiTask", 8 * configMINIMAL_STACK_SIZE, NULL, 1, &xWIFITaskHandle);
	}

	switch (config->service) {
		case WIFI_SERVICE_NTP:
			xWIFI_CIPMUX = WIFI_SINGLE_CONNECTION;
			NTP_Init();
			break;
		case WIFI_SERVICE_MQTT:
			xWIFI_CIPMUX = WIFI_SINGLE_CONNECTION;
			MQTT_Init(config->xMQTT_ConfigCallback);
			break;
		case WIFI_SERVICE_BOTH:
			xWIFI_CIPMUX = WIFI_MULTIPLE_CONNECTION;
			NTP_Init();
			MQTT_Init(config->xMQTT_ConfigCallback);
			break;
		default:
			break;
    }
}

bool WIFI_Ready( void ) {
	return xWIFIQueue != NULL;
}

void WIFI_Connect( void ) {
	WIFI_QueueItem item = { .cmd = WIFI_CMD_CONNECT };
	if (xWIFIQueue != NULL) {
		xQueueSend(xWIFIQueue, &item, portMAX_DELAY);
	}
}

void WIFI_Disconnect( void ) {
	WIFI_QueueItem item = { .cmd = WIFI_CMD_DISCONNECT };
	if (xWIFIQueue != NULL) {
		xQueueSend(xWIFIQueue, &item, portMAX_DELAY);
	}
}

bool WIFI_RequestInfo( void ) {

	if(MQTT_GetStatus() == MQTT_PROCESSING) return false;

	WIFI_QueueItem item = {
			.cmd = WIFI_CMD_GET_INFO
	};
	if (xWIFIQueue != NULL) {
		WIFI_SetResult(WIFI_RESULT_NONE);
		xQueueSend(xWIFIQueue, &item, portMAX_DELAY);
	}
	return true;
}

bool WIFI_RequestExpected( char *command, char *expected, int timeout_ms ){

	if(MQTT_GetStatus() == MQTT_PROCESSING) return false;

	WIFI_QueueItem item = {
			.cmd = WIFI_CMD_GET_EXPECTED,
			.timeoutMS = timeout_ms,
			.command = command,
			.user_data = expected
	};

	if( xWIFIQueue != NULL ){
		WIFI_SetResult(WIFI_RESULT_NONE);
		xQueueSend(xWIFIQueue, &item, portMAX_DELAY);
	}
	return true;
}

bool WIFI_RequestSendPacket( char *command, uint8_t *data, int length, int timeout_ms ) {

	if (MQTT_GetStatus() == MQTT_PROCESSING) return false;

	WIFI_QueueItem item = {
			.cmd = WIFI_CMD_SEND,
			.command = command,
			.packet = data,
			.length = length,
			.timeoutMS = timeout_ms
	};

	if (xWIFIQueue != NULL) {
		WIFI_SetResult(WIFI_RESULT_NONE);
		xQueueSend(xWIFIQueue, &item, portMAX_DELAY);
	}
	return true;
}

bool WIFI_RequestReceive( int timeout_ms, WIFI_ParserFunc parser_recv, void *uData ){

	if(MQTT_GetStatus() != MQTT_PROCESSING) return false;

	WIFI_QueueItem item ={
			.cmd = WIFI_CMD_RECEIVE,
			.timeoutMS = timeout_ms,
			.parser = parser_recv,
			.user_data = uData
	};

	if( xWIFIQueue != NULL ){
		WIFI_SetResult(WIFI_RESULT_NONE);
		xQueueSend(xWIFIQueue, &item, portMAX_DELAY);
	}
	return true;
}

bool WIFI_RequestPacketTransaction( char *command, uint8_t *data, int length,
		int timeout_ms, WIFI_ParserFunc parser_recv, void *uData ){

	if (MQTT_GetStatus() == MQTT_PROCESSING) return false;

	WIFI_QueueItem item = {
			.cmd = WIFI_CMD_TRANSACTION,
			.command = command,
			.packet = data,
			.length = length,
			.timeoutMS = timeout_ms,
			.parser = parser_recv,
			.user_data = uData
	};

	if (xWIFIQueue != NULL) {
		WIFI_SetResult(WIFI_RESULT_NONE);
		xQueueSend(xWIFIQueue, &item, portMAX_DELAY);
	}
	return true;
}

WIFI_ConnectionModeValueType WIFI_GetCIPMUX( void ){
	return xWIFI_CIPMUX;
}

WIFI_StateValueType WIFI_GetState( void ){
	WIFI_StateValueType state = WIFI_STATE_DISCONNECTED;
	if(xWIFIMutex != NULL){
		if(xSemaphoreTake(xWIFIMutex, WIFI_GET_STATE_TIMEOUT) == pdTRUE){
			state = xWIFI_State;
			xSemaphoreGive(xWIFIMutex);
		}
	}
	return state;
}

WIFI_ResultValueType WIFI_GetResult( void ){
	WIFI_ResultValueType result = WIFI_RESULT_NONE;
	if(xWIFIMutex != NULL){
		if(xSemaphoreTake(xWIFIMutex, WIFI_GET_STATE_TIMEOUT) == pdTRUE){
			result = xWIFI_Result;
			xSemaphoreGive(xWIFIMutex);
		}
	}
	return result;
}

bool WIFI_GetSavedIP( char *buffer ){
	if( buffer == NULL ) return false;

	if( xWIFIMutex != NULL ){
		if (xSemaphoreTake(xWIFIMutex, WIFI_GET_STATE_TIMEOUT) == pdTRUE){
			strncpy(buffer, cWIFI_IP, WIFI_MAX_IPv4_LENGTH);
			buffer[WIFI_MAX_IPv4_LENGTH - 1] = '\0';
			xSemaphoreGive(xWIFIMutex);
			return true;
		}
	}
	return false;
}

bool WIFI_GetSavedMAC( char *buffer ){
	if( buffer == NULL ) return false;

	if(xWIFIMutex != NULL) {
		if(xSemaphoreTake(xWIFIMutex, WIFI_GET_STATE_TIMEOUT) == pdTRUE) {
			strncpy(buffer, cWIFI_MAC, WIFI_MAX_MAC_LENGTH);
			buffer[WIFI_MAX_MAC_LENGTH - 1] = '\0';
			xSemaphoreGive(xWIFIMutex);
			return true;
		}
	}
	return false;
}

bool WIFI_WaitConnect( void ){

	WIFI_Connect();

	TickType_t xStart = xTaskGetTickCount();
	while ((xTaskGetTickCount() - xStart) < WIFI_CONN_SUCESS_TIMEOUT) {

		if (WIFI_GetState() == WIFI_STATE_CONNECTED) return true;

		vTaskDelay(WIFI_WAIT_DELAY);
	}
	return false;
}

bool WIFI_WaitDisconnect( void ){

	WIFI_Disconnect();

	TickType_t xStart = xTaskGetTickCount();
	while ((xTaskGetTickCount() - xStart) < WIFI_DCONN_TIMEOUT) {

		if (WIFI_GetState() != WIFI_STATE_CONNECTED) return true;

		vTaskDelay(WIFI_WAIT_DELAY);
	}

	return false;
}


bool WIFI_WaitIP( char *buffer ){

	if( buffer == NULL ) return false;

	WIFI_ResultValueType result;

	if(!WIFI_RequestInfo()) return false;

	TickType_t xStart = xTaskGetTickCount();
	while ((xTaskGetTickCount() - xStart) < WIFI_GET_IP_TIMEOUT){

		if(((result = WIFI_GetResult()) == WIFI_RESULT_SUCCESS)){
			if (WIFI_GetSavedIP(buffer)) {
				if(strcmp(buffer, WIFI_IPv4_DEFAULT) != 0) return true;
			}
		}

		if (result == WIFI_RESULT_ERROR) {
			break;
		}

		vTaskDelay(WIFI_WAIT_DELAY);
	}

	return false;
}

bool WIFI_WaitMAC( char *buffer ){

	if( buffer == NULL ) return false;

	WIFI_ResultValueType result;

	if(!WIFI_RequestInfo()) return false;

	TickType_t xStart = xTaskGetTickCount();
	while ((xTaskGetTickCount() - xStart) < WIFI_GET_MAC_TIMEOUT){
		if(((result = WIFI_GetResult()) == WIFI_RESULT_SUCCESS)){
			if (WIFI_GetSavedMAC(buffer)) {
				if(strcmp(buffer, WIFI_MAC_DEFAULT) != 0) return true;
			}
		}

		if (result == WIFI_RESULT_ERROR) {
			break;
		}

		vTaskDelay(WIFI_WAIT_DELAY);
	}

	return false;
}
