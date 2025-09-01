/*
 * flashcontrol.c
 *
 *  Created on: 26/11/2024
 *      Author: André Vaz (48209) e Emilio Mendes (48170)
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "flash.h"
#include "flashcontrol.h"


int FLASHCONTROL_EraseSector( unsigned int sector ){
	return FLASH_EraseSector ( sector );
}

int FLASHCONTROL_WriteData(void *dstAddr, void *srcAddr, unsigned int size) {
    unsigned int DstAddr = (unsigned int)dstAddr;
    unsigned int bufferSize;

    if (size == 0) return COUNT_ERROR;

    if (size <= 256) {
        bufferSize = 256;
    } else if (size <= 512) {
        bufferSize = 512;
    } else if (size <= 1024) {
        bufferSize = 1024;
    } else if (size <= 4096) {
        bufferSize = 4096;
    } else {
        return COUNT_ERROR;
    }

    if (DstAddr < 0x00000000 || DstAddr > (0x00080000 - bufferSize)) {
        return DST_ADDR_ERROR;
    }

    char *buffer = (char *)malloc(bufferSize);
    if (buffer == NULL) {
        return -1;
    }

    memcpy(buffer, srcAddr, size);

    int res = FLASH_WriteData(dstAddr, buffer, bufferSize);

    free(buffer);

    return res;
}


int FLASHCONTROL_VerifyData(void *dstAddr, void *srcAddr, unsigned int size) {
    if (size == 0) return COUNT_ERROR;
    if (size > 4096) return COUNT_ERROR;

    unsigned int bufferSize = size;

    if (size % 4 != 0) {
        bufferSize += 4 - (size % 4);
    }

    if (((unsigned int)dstAddr % 4) != 0) return DST_ADDR_ERROR;
    if (((unsigned int)srcAddr % 4) != 0) return SRC_ADDR_ERROR;

    unsigned int res = FLASH_VerifyData(dstAddr, srcAddr, bufferSize);
    return res;
}

char* FLASHCONTROL_GetResultMsg( int resultValue ){
	switch(resultValue){
	case 0:
		return COMMAND_SUCCESS_MSG;
	case 1:
		return INVALID_COMMAND_MSG;
	case 2:
		return SRC_ADDR_ERROR_MSG;
	case 3:
		return DST_ADDR_ERROR_MSG;
	case 4:
		return SRC_ADDR_NOT_MAPPED_MSG;
	case 5:
		return DST_ADDR_NOT_MAPPED_MSG;
	case 6:
		return COUNT_ERROR_MSG;
	case 7:
		return INVALID_SECTOR_MSG;
	case 8:
		return SECTOR_NOT_BLANK_MSG;
	case 9:
		return SECTOR_NOT_PREPARED_MSG;
	case 10:
		return COMPARE_ERROR_MSG;
	case 11:
		return BUSY_MSG;
	}
	return NULL;
}


