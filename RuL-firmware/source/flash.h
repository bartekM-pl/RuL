/*
 * flash.h
 *
 *  Created on: 22 mar 2019
 *      Author: bartek
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <stdbool.h>

#define RDSR1 0x05
#define RDSR2 0x07
#define WREN  0x06
#define WRDI  0x04
#define PP4   0x12	//Page Program (4B address)
#define CE	  0x60
#define READ4 0x13

#define PAGE_SIZE 8	//ile bajtów ma strona w formacie 2^n (n=8 ->256B, n=9 ->512B)


void FLASH_init();
void FLASH_1byteCommand(uint8_t value);
void FLASH_2byteCommand(uint16_t value);
void FLASH_WriteEnable(uint8_t value);
bool SPI_MemoryCheck();
void FLASH_writeByte(uint32_t address, uint8_t data);
void FLASH_writePage(uint32_t page, uint8_t * data);
uint8_t FLASH_readByte(uint32_t add);
void FLASH_readPage(uint32_t page, uint8_t * data);
void FLASH_chipErase();
void FLASH_sectorErase(uint32_t address);
uint8_t FLASH_status1();
uint8_t FLASH_status2();
void FLASH_waitForReady();
//uint8_t FLASH_write(sensors_t * sens);

#endif /* FLASH_H_ */
