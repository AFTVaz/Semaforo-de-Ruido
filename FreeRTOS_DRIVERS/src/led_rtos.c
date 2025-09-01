/*
 * led_rtos.c
 *
 *  Created on: 17/03/2025
 *      Author: André Vaz (48209) e Filipe Cruz (43468)
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "led.h"
#include "led_rtos.h"

static SemaphoreHandle_t xLedMutex = NULL;
static TaskHandle_t xLedTaskHandle = NULL;
static QueueHandle_t xLedQueue = NULL;

static void LED_RTOS_Task(void *pvParameters) {
    LED_RTOS_Command *cmd;
    while (true) {
        if (xQueueReceive(xLedQueue, &cmd, portMAX_DELAY) == pdTRUE) {
            switch (cmd->command) {
                case LED_CMD_ON:
                    LED_On(cmd->color, cmd->model);
                    break;
                case LED_CMD_OFF:
                    LED_Off(cmd->color, cmd->model);
                    break;
                case LED_CMD_TOGGLE:
                    LED_Toggle(cmd->color, cmd->model);
                    break;
            }
            vPortFree(cmd);
        }
    }
}

static void LED_RTOS_Init( void ) {
    if (xLedQueue == NULL){
    	xLedQueue = xQueueCreate(10, sizeof(LED_RTOS_Command *));
    	vQueueAddToRegistry(xLedQueue , "LQR");
    }

    if (xLedTaskHandle == NULL){
        xTaskCreate(LED_RTOS_Task, "LED_RTOS_Task",
                    configMINIMAL_STACK_SIZE, NULL, 1, &xLedTaskHandle);
    }

    if (xLedMutex == NULL){
        xLedMutex = xSemaphoreCreateMutex();
    }
}


void LED_RTOS_InitRedInt( bool state, LED_ModelType model ) {
	LED_Init(LED_INT_RED, state, model);
	LED_RTOS_Init();
}

void LED_RTOS_InitAllInt( bool state, LED_ModelType model ) {
	LED_Init(LED_INT_RED, state, model);
	LED_Init(LED_INT_GREEN, state, model);
	LED_Init(LED_INT_BLUE, state, model);
	LED_RTOS_Init();
}

void LED_RTOS_InitAllExt( bool state, LED_ModelType model ) {
	LED_Init(LED_EXT_RED, state, model);
	LED_Init(LED_EXT_GREEN, state, model);
	LED_Init(LED_EXT_BLUE, state, model);
	LED_RTOS_Init();
}

void LED_RTOS_InitSemaphore( LED_ModelType model ) {
	LED_Init(LED_SEM_RED, false, model);
	LED_Init(LED_SEM_YELLOW, false, model);
	LED_Init(LED_SEM_GREEN, true, model);
	LED_RTOS_Init();
}

static LED_RTOS_Command *LED_RTOS_AllocCommand(void) {
    return pvPortMalloc(sizeof(LED_RTOS_Command));
}

bool LED_RTOS_Ready( void ) {
	return (xLedTaskHandle != NULL) && (xLedQueue != NULL);
}

bool LED_RTOS_GetState(LED_ColorValueType color, bool *ledState) {
	if (ledState == NULL || xLedMutex == NULL) return false;

	for (int mutexRetry = 0; mutexRetry < LED_MAX_MUTEX_RETRY; ++mutexRetry) {
		if (xSemaphoreTake(xLedMutex, LED_GETSTATE_TIMEOUT) == pdTRUE) {
			*ledState = LED_GetState(color);
			xSemaphoreGive(xLedMutex);
			return true;
		}
		vTaskDelay(LED_GETSTATE_DELAY);
	}

	return false;
}

void LED_RTOS_On(LED_ColorValueType color, LED_ModelType model) {
	if (!LED_RTOS_Ready()) return;
    LED_RTOS_Command *cmd = LED_RTOS_AllocCommand();
    if (cmd == NULL) return;
    cmd->color = color;
    cmd->command = LED_CMD_ON;
    cmd->model = model;
    if (xQueueSend(xLedQueue, &cmd, portMAX_DELAY) != pdTRUE) {
    	vPortFree(cmd);
    }
}

void LED_RTOS_Off(LED_ColorValueType color, LED_ModelType model) {
	if (!LED_RTOS_Ready()) return;
    LED_RTOS_Command *cmd = LED_RTOS_AllocCommand();
    if (cmd == NULL) return;
    cmd->color = color;
    cmd->command = LED_CMD_OFF;
    cmd->model = model;
    if (xQueueSend(xLedQueue, &cmd, portMAX_DELAY) != pdTRUE) {
    	vPortFree(cmd);
    }
}

void LED_RTOS_Toggle(LED_ColorValueType color, LED_ModelType model) {
	if (!LED_RTOS_Ready()) return;
    LED_RTOS_Command *cmd = LED_RTOS_AllocCommand();
    if (cmd == NULL) return;
    cmd->color = color;
    cmd->command = LED_CMD_TOGGLE;
    cmd->model = model;
    if (xQueueSend(xLedQueue, &cmd, portMAX_DELAY) != pdTRUE) {
        vPortFree(cmd);
    }
}

void LED_RTOS_Delete(void) {
    if (xLedTaskHandle != NULL) {
        vTaskDelete(xLedTaskHandle);
        xLedTaskHandle = NULL;
    }

    if (xLedQueue != NULL) {
        vQueueDelete(xLedQueue);
        xLedQueue = NULL;
    }

    if (xLedMutex != NULL) {
        vSemaphoreDelete(xLedMutex);
        xLedMutex = NULL;
    }
}






