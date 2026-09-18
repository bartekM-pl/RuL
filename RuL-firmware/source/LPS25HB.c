/*
 * LPS25HB.c
 *
 *  Created on: 28 mar 2019
 *      Author: b.moczala
 */

#include <stdio.h>
#include <stdlib.h>
#include "fsl_common.h"
#include "MKL27Z4.h"
#include "struct.h"
#include "spi.h"
#include "LPS25HB.h"


void LPS_writeReg(uint8_t reg, uint8_t val){
	uint8_t buf[2] = {reg&(~0xC0), val};	//zerowanie MSB bo zapis i bitu 7 bo nie zmieniamy adresu

	SPI_CS_LPS(1);
	SPI_RW(buf, buf, 2);
	SPI_CS_LPS(0);
}

uint8_t LPS_readReg1(uint8_t reg){
	uint8_t buf[2] = {(reg&(~0x40))|0x80, 0};	//ustawienie MSB bo odczyt i zerowanie MSB bo zapis i bitu 7 bo nie zmieniamy adresu

	SPI_CS_LPS(1);
	SPI_RW(buf, buf, 2);
	SPI_CS_LPS(0);

	return buf[1];
}


void LPS_readRegN(uint8_t reg, uint8_t * val, uint8_t length){
	uint8_t buf[32];
	memset(buf, 0, 32);
	buf[0] = (reg&(~0x40))|0x80;	//ustawienie MSB bo odczyt i zerowanie MSB bo zapis i bitu 7 bo nie zmieniamy adresu

	SPI_CS_LPS(1);
	SPI_RW(buf, val, length);
	SPI_CS_LPS(0);
}

float LPS_readPress(){
	uint8_t buf[6] = {0, 0, 0, 0, 0, 0};
	buf[0] = 0x28 | 0x80 | 0x40;	//ustawienie MSB bo odczyt  i bitu 7 bo zmieniamy adres

	SPI_CS_LPS(1);
	SPI_RW(buf, buf, 6);
	SPI_CS_LPS(0);

	uint32_t raw = ((uint32_t)buf[3]) << 16 | ((uint32_t)buf[2]) << 8 | ((uint32_t)buf[1]);

	return (float)raw/40.960;	//Pa
}

float LPS_readTemp(){
	uint8_t buf[4] = {0, 0, 0, 0};
	buf[0] = 0x2B | 0x80 | 0x40;	//ustawienie MSB bo odczyt  i bitu 7 bo zmieniamy adres

	SPI_CS_LPS(1);
	SPI_RW(buf, buf, 3);
	SPI_CS_LPS(0);

	int16_t raw =  (uint16_t)buf[2] << 8 | (uint16_t)buf[1];

	return 42.5f + (float)raw/480.0f;	//C degree
}

void LPS_update(sensors_t * val){
	val->pressure = LPS_readPress();
	val->temp = LPS_readTemp();
	val->altitude = 0.0f;
}

void LPS_init(){
	LPS_writeReg(0x10, 0x0F);	//RES_CONF - Temp avg = 8, Press avg = 8	//0x0F dla Temp avg = 64, Press avg = 512
	LPS_writeReg(0x20, 0xC0);	//CTRL_REG1 - Active mode, ODR = 25Hz,  No autozero
	//LPS_writeReg(0x20, 0x90);	//CTRL_REG1 - Active mode, ODR = 1Hz,  No autozero
	//LPS_writeReg(0x21, 0x08);	//CTRL_REG2 - I2C disabled
	//LPS_writeReg(0x22, 0x80);	//CTRL_REG3 - INT active low, INT push pull, INT on data signal
	LPS_writeReg(0x23, 0x01);	//CTRL_REG4 - INT on new data
	//LPS_writeReg(0x24, 0x00);	//INTERRUPT_CFG - no additional INT
}

uint8_t LPS_WhoIam(){
	uint8_t buf[2] = {0};
	buf[0] = (0x0F & (~0x40)) | 0x80;	//ustawienie MSB bo odczyt i zerowanie MSB bo zapis i bitu 7 bo nie zmieniamy adresu

	SPI_CS_LPS(1);
	SPI_RW(buf, buf, 2);
	SPI_CS_LPS(0);

	return buf[1];	//0xBD
}
