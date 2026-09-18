/*
 * i2c.c
 *
 *  Created on: 22 mar 2019
 *      Author: bartek
 */


#include <stdio.h>
#include <stdlib.h>
#include "MKL27Z4.h"
#include "peripherals.h"
#include "fsl_i2c.h"
#include "i2c.h"

volatile bool g_MasterCompletionFlag = false;
i2c_master_transfer_t masterXfer;

void i2c_master_callback(I2C_Type * base, i2c_master_dma_handle_t * handle, status_t status, void * userData){
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success){
        g_MasterCompletionFlag = true;
    }
}

void I2C_Send(uint8_t devAddress, uint32_t subAddress, uint8_t subAddressLength, uint8_t * data, uint16_t length){
	masterXfer.slaveAddress = devAddress;
	masterXfer.direction = kI2C_Write;
	masterXfer.subaddress = subAddress;
	masterXfer.subaddressSize = subAddressLength;
	masterXfer.data = data;
	masterXfer.dataSize = length;
	masterXfer.flags = kI2C_TransferDefaultFlag;

	g_MasterCompletionFlag = false;
	I2C_MasterTransferDMA(I2C0_PERIPHERAL, &I2C0_DMA_Handle, &masterXfer);
	while(g_MasterCompletionFlag != true) {}
}

void I2C_Read(uint8_t devAddress, uint32_t subAddress, uint8_t subAddressLength, uint8_t * data, uint16_t length){
	masterXfer.slaveAddress = devAddress;
	masterXfer.direction = kI2C_Read;
	masterXfer.subaddress = subAddress;
	masterXfer.subaddressSize = subAddressLength;
	masterXfer.data = data;
	masterXfer.dataSize = length-1;	//???????????????????
	masterXfer.flags = kI2C_TransferDefaultFlag;

	g_MasterCompletionFlag = false;
	I2C_MasterTransferDMA(I2C0_PERIPHERAL, &I2C0_DMA_Handle, &masterXfer);
	while(g_MasterCompletionFlag != true) {}
}
