#include <time.h>
#include <stdio.h>
#include <stdbool.h>

/* Kernel includes. */
#include "FreeRTOS.h"


/* Kernel includes. */
#include "task.h"
#include "queue.h"


/* Drivers */

int main(void)
{
	vTaskStartScheduler();
	while(1);
	return 0;
}

/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	/* This function will get called if a task overflows its stack. */

	( void ) pxTask;
	( void ) pcTaskName;
	printf("Stack overflow: %s\n", pcTaskName);
	for( ;; );
}

/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* This function will get called if a malloc fail. */

	printf("Fail memory alloc\n");
	for( ;; );
}
