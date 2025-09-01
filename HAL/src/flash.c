/*
 * flash.c
 *
 *  Created on: 19/11/2024
 *      Author: André Vaz (48209) e Emilio Mendes (48170)
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdio.h>
#include <stdint.h>
#include "flash.h"

#define IAP_LOCATION 0x1FFF1FF1
#define FLASH_FREQ (SystemCoreClock/1000)

typedef void (*IAP)(unsigned int[], unsigned int[]);
IAP iap_entry = (IAP) IAP_LOCATION;

static unsigned int FLASH_GetSectorFromAddress(unsigned int address) {
    if (address < 0x00010000) {
        // 4KB sectors (0x0000 0000 to 0x0000 FFFF)
        return address / 0x1000; // 4 KB = 0x1000
    } else if (address < 0x00080000) {
        // 32KB sectors (0x0001 0000 to 0x0007 FFFF)
        return 16 + ((address - 0x00010000) / 0x8000); // 32 KB = 0x8000
    } else {
        return 0xFFFFFFFF; // Invalid address
    }
}

unsigned int FLASH_EraseSector ( unsigned int sector ){

	unsigned int command[5];
	unsigned int result[5];

	command[0] = Prepare_Sectors_For_Write;
	command[1] = sector;
	command[2] = sector;

	iap_entry(command, result);

	if(result[0] != CMD_SUCCESS) return result[0];

	command[0] = Erase_Sectors;
	command[1] = sector;
	command[2] = sector;
	command[3] = FLASH_FREQ;

	iap_entry(command, result);

	return result[0];

}

unsigned int FLASH_WriteData(void *dstAddr, void *srcAddr, unsigned int size) {
    unsigned int command[5];
    unsigned int result[5];
    unsigned int DstAddr = (unsigned int)dstAddr;
    unsigned int SrcAddr = (unsigned int)srcAddr;

    unsigned int sector = FLASH_GetSectorFromAddress(DstAddr);
    if (sector == 0xFFFFFFFF) return INVALID_SECTOR;

    command[0] = Prepare_Sectors_For_Write;
    command[1] = sector;
    command[2] = sector;

    iap_entry(command, result);
    if (result[0] != CMD_SUCCESS) return result[0];

    command[0] = COPY_RAM_TO_FLASH;
    command[1] = DstAddr;
    command[2] = SrcAddr;
    command[3] = size;
    command[4] = FLASH_FREQ;

    iap_entry(command, result);

    return result[0];
}

unsigned int FLASH_VerifyData ( void *dstAddr , void *srcAddr , unsigned int size ){

	unsigned int command[5];
	unsigned int result[5];
	unsigned int DstAddr = (unsigned int)dstAddr;
	unsigned int SrcAddr = (unsigned int)srcAddr;

	command[0] = Compare_Addr1_Addr2_nBytes;
	command[1] = DstAddr;
	command[2] = SrcAddr;
	command[3] = size;

	iap_entry(command, result);

	return result[0];

}
