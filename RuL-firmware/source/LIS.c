/*
 * LIS.c
 *
 *  Created on: 31 mar 2019
 *      Author: bartek
 */

#include <stdio.h>
#include <stdlib.h>
#include "fsl_common.h"
#include "MKL27Z4.h"
#include "struct.h"
#include "spi.h"
#include "LIS.h"


void LIS_writeReg(uint8_t reg, uint8_t val){
	uint8_t buf[2] = {reg & (~0x80), val};	//zerowanie MSB bo zapis

	SPI_CS_LIS(1);
	SPI_RW(buf, buf, 2);
	SPI_CS_LIS(0);
}

uint8_t LIS_readReg1(uint8_t reg){
	uint8_t buf[2] = {reg | 0x80, 0};	//ustawienie MSB bo odczyt

	SPI_CS_LIS(1);
	SPI_RW(buf, buf, 2);
	SPI_CS_LIS(0);

	return buf[1];
}


void LIS_readRegN(uint8_t reg, uint8_t * val, uint8_t length){
	uint8_t buf[32];
	memset(buf, 0, 32);
	buf[0] = reg | 0x80;	//ustawienie MSB bo odczyt

	SPI_CS_LIS(1);
	SPI_RW(buf, val, length);
	SPI_CS_LIS(0);
}

void LIS_readAcc(sensors_t * val){
	uint8_t buf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	buf[0] = 0x28 | 0x80;	//ustawienie MSB bo odczyt

	SPI_CS_LIS(1);
	SPI_RW(buf, buf, 7);	//1B reg + 6B data
	SPI_CS_LIS(0);

	int16_t AccX =  (uint16_t)buf[2] << 8 | (uint16_t)buf[1];
	int16_t AccY =  (uint16_t)buf[4] << 8 | (uint16_t)buf[3];
	int16_t AccZ =  (uint16_t)buf[6] << 8 | (uint16_t)buf[5];


	val->acc.x = (float)AccX / 2048.0f;
	val->acc.y = (float)AccY / 2048.0f;
	val->acc.z = (float)AccZ / 2048.0f;
}

void LIS_update(sensors_t * val){
	LIS_readAcc(val);
}

void LIS_init(){

	LIS_writeReg(0x20, 0x31);	//CTRL1 - ODR = 25Hz, Low Power Mode 2, 14bit res
	//LIS_writeReg(0x20, 0x11);	//CTRL1 - ODR = 12.5/1.6Hz, Low Power Mode 2, 14bit res
	LIS_writeReg(0x21, 0x0E);	//CTRL2 - I2C disabled, BDU, address inc
	//LIS_writeReg(0x22, 0x08);	//CTRL3 - INT active low, INT push pull, <<-----------możliwe, że LIR też trzeba ustawić
	//LIS_writeReg(0x23, 0x01);	//CTRL4_INT1_PAD_CTRL - INT on new data
	//LIS_writeReg(0x24, 0x00);	//CTRL5_INT2_PAD_CTRL - no additional INT
	LIS_writeReg(0x25, 0x34);	//CTRL6 - Filter = ODR/2, Range +/- 16g, Low Pass Filter, Low Noise
	//LIS_writeReg(0x2F, 0x30);	//CTRL7 - Latch Mode INT, INT enabled
}

uint8_t LIS_WhoIam(){
	uint8_t buf[2] = {0};
	buf[0] = (0x0F & (~0x40)) | 0x80;	//ustawienie MSB bo odczyt i zerowanie MSB bo zapis i bitu 7 bo nie zmieniamy adresu

	SPI_CS_LIS(1);
	SPI_RW(buf, buf, 2);
	SPI_CS_LIS(0);

	return buf[1];	//0x44
}
