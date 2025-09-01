/*
 * flash_rtos.c
 *
 *  Created on: 13/06/2025
 *      Author: André Vaz (48209) e Filipe Cruz (43468)
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "flashcontrol.h"
#include "flash_rtos.h"

static SemaphoreHandle_t xFlashMutex = NULL;

void FLASH_RTOS_Init( void ){
    if(xFlashMutex == NULL){
        xFlashMutex = xSemaphoreCreateMutex();
    }
}

int FLASH_RTOS_EraseSector( unsigned int sector ){
    if(xFlashMutex == NULL) return FLASH_RTOS_ERROR;

    int result = FLASH_RTOS_ERROR;

    if(xSemaphoreTake(xFlashMutex, portMAX_DELAY) == pdTRUE){
        result = FLASHCONTROL_EraseSector(sector);
        xSemaphoreGive(xFlashMutex);
    }

    return result;
}

int FLASH_RTOS_WriteData( void *dstAddr, void *srcAddr, unsigned int size ) {
    if(xFlashMutex == NULL) return FLASH_RTOS_ERROR;

    int result = FLASH_RTOS_ERROR;

    if(xSemaphoreTake(xFlashMutex, portMAX_DELAY) == pdTRUE){
        result = FLASHCONTROL_WriteData(dstAddr, srcAddr, size);
        xSemaphoreGive(xFlashMutex);
    }

    return result;
}

int FLASH_RTOS_VerifyData( void *dstAddr, void *srcAddr, unsigned int size ) {
    if(xFlashMutex == NULL) return FLASH_RTOS_ERROR;

    int result = FLASH_RTOS_ERROR;

    if(xSemaphoreTake(xFlashMutex, portMAX_DELAY) == pdTRUE){
        result = FLASHCONTROL_VerifyData(dstAddr, srcAddr, size);
        xSemaphoreGive(xFlashMutex);
    }

    return result;
}
