/*
 * spi.h
 *
 *  Created on: 22 mar 2019
 *      Author: bartek
 */

#ifndef SPI_H_
#define SPI_H_

void SPI_SPI_FLASHMODE(uint8_t onoff);
void SPI_CS_LPS(uint8_t val);
void SPI_CS_LIS(uint8_t val);
void SPI_CS_FLASH(uint8_t val);
void SPI_RW(uint8_t * in, uint8_t * out, uint16_t length);



#endif /* SPI_H_ */
