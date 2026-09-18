/*
 * spi.c
 *
 *  Created on: 22 mar 2019
 *      Author: bartek
 */


#include <stdio.h>
#include <stdlib.h>
#include "MKL27Z4.h"
#include "peripherals.h"
#include "fsl_spi.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "spi.h"

spi_transfer_t xfer = {0};
static volatile bool masterFinished = false;

void SPI_0_masterCallback(SPI_Type *base, spi_dma_handle_t *handle, status_t status, void *userData){
    masterFinished = true;
}

void SPI_SPI_FLASHMODE(uint8_t onoff){
	if(onoff){
		PORT_SetPinMux(PORTC, 6U, kPORT_MuxAlt5); /* PORTC6 (pin 27) is configured as SPI0_MISO */
		PORT_SetPinMux(PORTC, 7U, kPORT_MuxAlt5); /* PORTC7 (pin 28) is configured as SPI0_MOSI */
	} else {
		PORT_SetPinMux(PORTC, 6U, kPORT_MuxAlt2); /* PORTC6 (pin 27) is configured as SPI0_MOSI */
		PORT_SetPinMux(PORTC, 7U, kPORT_MuxAlt2); /* PORTC7 (pin 28) is configured as SPI0_MISO */
	}
}

void SPI_CS_LPS(uint8_t val){
	GPIO_PinWrite(GPIOC, 4, val?0:1);
}

void SPI_CS_LIS(uint8_t val){
	GPIO_PinWrite(GPIOC, 3, val?0:1);
}

void SPI_CS_FLASH(uint8_t val){
	GPIO_PinWrite(GPIOD, 4, val?0:1);
}

void SPI_RW(uint8_t * in, uint8_t * out, uint16_t length){
	/* Send to slave */
	xfer.txData = in;
	xfer.rxData = out;
	xfer.dataSize = length;

	masterFinished = false;
	SPI_MasterTransferDMA(SPI0_PERIPHERAL, &SPI0_DMA_Handle, &xfer);
	while(masterFinished != true) {}
}
