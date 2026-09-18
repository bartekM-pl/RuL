/*
 * FS.c
 *
 *  Created on: 10 maj 2019
 *      Author: b.moczala
 */


#include <stdio.h>
#include <stdlib.h>
#include "MKL27Z4.h"
#include "flash.h"
#include "FS.h"

uint8_t FS_testWR(uint32_t address){
	FLASH_writeByte(address, 0xAA);
	uint8_t val = FLASH_readByte(address);

	if(val == 0xAA) return 0;
	else return 1;
}


void FS_initFS(){

}

void FS_getFilesFromTable(){

}

void FS_getFilesFromPages(){

}
