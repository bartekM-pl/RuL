/*
 * LIS.h
 *
 *  Created on: 31 mar 2019
 *      Author: bartek
 */

#ifndef LIS_H_
#define LIS_H_

#include "struct.h"

void LIS_writeReg(uint8_t reg, uint8_t val);
uint8_t LIS_readReg1(uint8_t reg);
void LIS_readRegN(uint8_t reg, uint8_t * val, uint8_t length);
void LIS_readAcc(sensors_t * val);
void LIS_update(sensors_t * val);
void LIS_init();
uint8_t LIS_WhoIam();

#endif /* LIS_H_ */
