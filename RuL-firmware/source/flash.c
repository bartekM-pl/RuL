/*
 * flash.c
 *
 *  Created on: 22 mar 2019
 *      Author: bartek
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "MKL27Z4.h"
#include "struct.h"
#include "spi.h"
#include "flash.h"
//------- Model pamięci S25FL064L -----------------------//

//static FLASH_pageStruct_t FLASH_pageStruct_d;

void FLASH_init(){
	SPI_CS_FLASH(1);
}


void FLASH_1byteCommand(uint8_t value){
	SPI_SPI_FLASHMODE(1);
	SPI_CS_FLASH(1);
	SPI_RW(&value, &value, 1);
	SPI_CS_FLASH(0);
	SPI_SPI_FLASHMODE(0);
}

void FLASH_WriteEnable(uint8_t value){
	if(value) FLASH_1byteCommand(WREN);
	else FLASH_1byteCommand(WRDI);
}

bool SPI_MemoryCheck(){
	uint8_t buf[4] = {0x9F, 0, 0, 0};

	SPI_SPI_FLASHMODE(1);
	SPI_CS_FLASH(1);
	SPI_RW(buf, buf, 4);
	SPI_CS_FLASH(0);
	SPI_SPI_FLASHMODE(0);

	if((buf[1] == 0x01) && (buf[2] == 0x60) && (buf[3] == 0x17)) return true;
	else return false;
}

void FLASH_writeByte(uint32_t add, uint8_t data){
	U32_8_t address;
	address.val32 = add;
	uint8_t buf[5] = {PP4, address.val8[3], address.val8[2], address.val8[1], address.val8[0]};

	FLASH_waitForReady();
	FLASH_WriteEnable(1);

	SPI_SPI_FLASHMODE(1);
	SPI_CS_FLASH(1);
	SPI_RW(buf, buf, 5);		//komenda PP4 i adres
	SPI_RW(&data, &data, 1);	//dane
	SPI_CS_FLASH(0);
	SPI_SPI_FLASHMODE(0);
}

void FLASH_writePage(uint32_t page, uint8_t * data){
	U32_8_t address;
	address.val32 = page<<PAGE_SIZE;
	uint8_t buf[5] = {PP4, address.val8[3], address.val8[2], address.val8[1], address.val8[0]};

	FLASH_waitForReady();
	FLASH_WriteEnable(1);

	SPI_SPI_FLASHMODE(1);
	SPI_CS_FLASH(1);
	SPI_RW(buf, buf, 5);				//komenda PP4 i adres
	SPI_RW(data, data, 2<<PAGE_SIZE);	//dane
	SPI_CS_FLASH(0);
	SPI_SPI_FLASHMODE(0);
}

uint8_t FLASH_readByte(uint32_t add){
	uint8_t data;
	U32_8_t address;
	address.val32 = add;
	uint8_t buf[5] = {READ4, address.val8[3], address.val8[2], address.val8[1], address.val8[0]};

	FLASH_waitForReady();

	SPI_SPI_FLASHMODE(1);
	SPI_CS_FLASH(1);
	SPI_RW(buf, buf, 5);		//komenda READ4 i adres
	SPI_RW(&data, &data, 1);	//dane
	SPI_CS_FLASH(0);
	SPI_SPI_FLASHMODE(0);

	return data;
}

void FLASH_readPage(uint32_t page, uint8_t * data){
	U32_8_t address;
	address.val32 = page<<PAGE_SIZE;
	uint8_t buf[5] = {READ4, address.val8[3], address.val8[2], address.val8[1], address.val8[0]};

	FLASH_waitForReady();

	SPI_SPI_FLASHMODE(1);
	SPI_CS_FLASH(1);
	SPI_RW(buf, buf, 5);				//komenda READ4 i adres
	SPI_RW(data, data, 2<<PAGE_SIZE);	//dane
	SPI_CS_FLASH(0);
	SPI_SPI_FLASHMODE(0);
}

void FLASH_chipErase(){
	FLASH_waitForReady();
	FLASH_WriteEnable(1);

	FLASH_1byteCommand(CE);
	FLASH_waitForReady();
}

void FLASH_sectorErase(uint32_t address){

}

uint8_t FLASH_status1(){
	uint8_t buf[2] = {RDSR1, 0};

	SPI_SPI_FLASHMODE(1);
	SPI_CS_FLASH(1);
	SPI_RW(buf, buf, 2);
	SPI_CS_FLASH(0);
	SPI_SPI_FLASHMODE(0);

	return buf[1];
}

uint8_t FLASH_status2(){
	uint8_t buf[2] = {RDSR2, 0};

	SPI_SPI_FLASHMODE(1);
	SPI_CS_FLASH(1);
	SPI_RW(buf, buf, 2);
	SPI_CS_FLASH(0);
	SPI_SPI_FLASHMODE(0);

	return buf[1];
}

void FLASH_waitForReady(){
	while(FLASH_status1() & 0x01) __asm__("nop");
}

uint32_t FLASH_FindEnd(){
//	SPI_CS(true);
//	SPI_W_Byte(0x03);			//Read
//	SPI_W_Byte(0);				//address MSB
//	SPI_W_Byte(0);				//address cd.
//	SPI_W_Byte(0);				//address LSB
//	uint32_t n=0;
//	while((n < 4100000) && (SPI_R_Byte() != 0xFF)) n++;	//szukaj pocztku wolnej pamici (0xFF)
//	SPI_CS(false);
//	if(n%length) n = n+length-n%length;
//	return n;
	return 0;
}


void FLASH_push(sensors_t * sens){

}

void FLASH_clearBuffer(){

}

void FLASH_writeBuffer(){
	//FLASH_writePage(uint32_t page, uint8_t * data);
}

bool FLASH_bufferFull(){
	//if(?) return true;
	return false;
}

//uint8_t FLASH_write(sensors_t * sens){
//	FLASH_push(sens);
//	if(FLASH_bufferFull()){
//		FLASH_writeBuffer();
//		FLASH_clearBuffer();
//	}
//
//	return 0;
//}
