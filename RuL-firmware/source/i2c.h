/*
 * i2c.h
 *
 *  Created on: 22 mar 2019
 *      Author: bartek
 */

#ifndef I2C_H_
#define I2C_H_

void I2C_Send(uint8_t devAddress, uint32_t subAddress, uint8_t subAddressLength, uint8_t * data, uint16_t length);
void I2C_Read(uint8_t devAddress, uint32_t subAddress, uint8_t subAddressLength, uint8_t * data, uint16_t length);

#endif /* I2C_H_ */
