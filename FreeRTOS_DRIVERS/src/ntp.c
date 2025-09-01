/*
 * ntp.c
 *
 *  Created on: 05/05/2025
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
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "espserial.h"
#include "wifi.h"
#include "ntp.h"

static TaskHandle_t xNTPTaskHandle = NULL;
static SemaphoreHandle_t xNTPMutex = NULL;
static time_t ntp_timestamp = 0;
static NTP_StateValueType xNTPState = NTP_IDLE;

static uint32_t ntohl( uint32_t netlong ){
	return __builtin_bswap32(netlong);
}

static bool NTP_ParseResponse( const char *buffer, void *user_data ){
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
				if(link_id != NTP_SERVICE_ID) return false;
			} else return false;

			if ( data_len == NTP_PACKET_SIZE ){
				NTP_Packet rxPacket;

				int len = ESPSERIAL_Recv((unsigned char *)&rxPacket, NTP_PACKET_SIZE);
				if ( len == NTP_PACKET_SIZE ){
					uint32_t secs_net = rxPacket.txTm_s;
					uint32_t secs = ntohl(secs_net);

					time_t *timestamp = (time_t *)user_data;
					*timestamp = (time_t)(secs - UNIX_EPOCH);
					return true;
				}
			}
		}
	}
	return false;
}

static bool NTP_Start( void ){
	WIFI_ResultValueType result;
	char cmd[128];

	int cipmux = WIFI_GetCIPMUX();

	if (cipmux == 0) {
		snprintf(cmd, sizeof(cmd), "AT+CIPSTART=\"%s\",\"%s\",%d\r\n",
				NTP_CONN_TYPE, NTP_SERVER, NTP_REMOTE_PORT);
	} else {
		snprintf(cmd, sizeof(cmd), "AT+CIPSTART=%d,\"%s\",\"%s\",%d\r\n",
				NTP_SERVICE_ID, NTP_CONN_TYPE, NTP_SERVER, NTP_REMOTE_PORT);
	}
	if(!WIFI_RequestExpected(cmd, "OK\r\n", NTP_CIPSTART_TIMEOUT)) return false;
	while ((result = WIFI_GetResult()) == WIFI_RESULT_NONE) taskYIELD();
	return result == WIFI_RESULT_SUCCESS;
}

static bool NTP_SendAndReceive( time_t *timestamp ) {
	if (timestamp == NULL) return false;

	WIFI_ResultValueType result;
	char cmd[128];
	NTP_Packet packet;
	memset(&packet, 0, sizeof(NTP_Packet));

	packet.li_vn_mode = 0xE3;  	// LI=3, VN=4, Mode=3
	packet.stratum = 0;     	// Stratum
	packet.poll = 6;     		// Poll interval
	packet.precision = 0xEC;  	// Precision

	int cipmux = WIFI_GetCIPMUX();

	if (cipmux == 0) {
		snprintf( cmd, sizeof(cmd), "AT+CIPSEND=%d\r\n", NTP_PACKET_SIZE );
	} else {
		snprintf( cmd, sizeof(cmd), "AT+CIPSEND=%d,%d\r\n", NTP_SERVICE_ID, NTP_PACKET_SIZE );
	}

	TickType_t xStart = xTaskGetTickCount();
	while ((xTaskGetTickCount() - xStart) < NTP_TRANSACTION_TIMEOUT){
		if(WIFI_RequestPacketTransaction(
				cmd,
				(uint8_t*)&packet,
				NTP_PACKET_SIZE,
				NTP_TRANSACTION_TIMEOUT,
				NTP_ParseResponse,
				timestamp)
		){
			while ((result = WIFI_GetResult()) == WIFI_RESULT_NONE) taskYIELD();
			return result == WIFI_RESULT_SUCCESS;
		}
		else vTaskDelay(NTP_WAIT_DELAY);
	}

	return false;
}

static bool NTP_Close( void ){
	WIFI_ResultValueType result;
	int cipmux = WIFI_GetCIPMUX();
	char cmd[32];

	if (cipmux == 0){
		snprintf(cmd, sizeof(cmd), "AT+CIPCLOSE\r\n");
	} else {
		snprintf(cmd, sizeof(cmd), "AT+CIPCLOSE=%d\r\n", NTP_SERVICE_ID);
	}

	while (!WIFI_RequestExpected(cmd, "OK\r\n", NTP_CIPCLOSE_TIMEOUT))
		vTaskDelay(NTP_WAIT_DELAY);

	while ((result = WIFI_GetResult()) == WIFI_RESULT_NONE) taskYIELD();
	return result == WIFI_RESULT_SUCCESS;
}

static void NTP_SetState( NTP_StateValueType state ) {
	if (xNTPMutex != NULL) {
		if (xSemaphoreTake(xNTPMutex, WIFI_SET_STATE_TIMEOUT) == pdTRUE) {
			xNTPState = state;
			xSemaphoreGive(xNTPMutex);
		}
	}
}

static void NTP_Task(void *pvParameters) {

	bool connStart = false;

	time_t timestamp = 0;

	while (true) {

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		if (WIFI_GetState() == WIFI_STATE_CONNECTED) {
			if (NTP_GetState() == NTP_PROCESSING) {
				if(!connStart) connStart = NTP_Start();

				if(connStart){
					if( NTP_SendAndReceive( &timestamp) ){
						if ( xSemaphoreTake(xNTPMutex, portMAX_DELAY) == pdTRUE ){
							ntp_timestamp = timestamp;
							xNTPState = NTP_READY;
							xSemaphoreGive(xNTPMutex);
						}
					} else {
						NTP_SetState( NTP_IDLE );
						NTP_Close();
						connStart = false;
					}
				}
				else{
					NTP_SetState( NTP_IDLE );
				}
			}
		} else {
		    if (NTP_GetState() != NTP_IDLE) {
		        NTP_SetState(NTP_IDLE);
		    }
		}
	}
}


void NTP_Init( void ) {
	xNTPMutex = xSemaphoreCreateMutex();
	if (xNTPMutex != NULL) {
		xTaskCreate(NTP_Task, "NTP_Task", 4 * configMINIMAL_STACK_SIZE, NULL, 1, &xNTPTaskHandle);
	}
}

bool NTP_Ready( void ){
	return xNTPTaskHandle != NULL;
}

NTP_StateValueType NTP_GetState( void ){
	NTP_StateValueType state = NTP_IDLE;
	if(xNTPMutex != NULL){
		if(xSemaphoreTake(xNTPMutex, WIFI_GET_STATE_TIMEOUT) == pdTRUE){
			state = xNTPState;
			xSemaphoreGive(xNTPMutex);
		}
	}
	return state;
}

void NTP_RequestTime( void ) {
	if ( NTP_GetState() == NTP_IDLE ) {
		NTP_SetState( NTP_PROCESSING );
		xTaskNotifyGive(xNTPTaskHandle);
	}
}

bool NTP_GetTimestamp( time_t *timestamp ){
	if (timestamp == NULL) return false;

	bool success = false;

	if (xSemaphoreTake(xNTPMutex, portMAX_DELAY) == pdTRUE) {
		if (xNTPState == NTP_READY) {
			time_t t = ntp_timestamp;
			if (t != 0) {
				*timestamp = t;
				success = true;
			}
			xNTPState = NTP_IDLE;
		}
		xSemaphoreGive(xNTPMutex);
	}
	return success;
}

bool NTP_RequestAndWait( time_t *timestamp ){

	TickType_t xStart = xTaskGetTickCount();

	NTP_RequestTime();
	while ((xTaskGetTickCount() - xStart) < NTP_TRANSACTION_TIMEOUT){
		if (NTP_GetTimestamp( timestamp )) {
			return true;
		}

		if (NTP_GetState() == NTP_IDLE) {
			break;
		}

		vTaskDelay(NTP_WAIT_DELAY);
	}

	return false;
}

