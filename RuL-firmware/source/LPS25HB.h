/*
 * LPS25HB.h
 *
 *  Created on: 28 mar 2019
 *      Author: b.moczala
 */

#ifndef LPS25HB_H_
#define LPS25HB_H_

#include "struct.h"

void LPS_writeReg(uint8_t reg, uint8_t val);
uint8_t LPS_readReg1(uint8_t reg);
void LPS_readRegN(uint8_t reg, uint8_t * val, uint8_t length);
float LPS_readPress();
float LPS_readTemp();
void LPS_update(sensors_t * val);
void LPS_init();
uint8_t LPS_WhoIam();

#endif /* LPS25HB_H_ */
