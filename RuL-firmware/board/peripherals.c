/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Peripherals v1.0
* BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/**
 * @file    peripherals.c
 * @brief   Peripherals initialization file.
 */
 
/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "peripherals.h"

/***********************************************************************************************************************
 * BOARD_InitPeripherals functional group
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * TPM_1 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'TPM_2'
- type: 'tpm'
- mode: 'EdgeAligned'
- type_id: 'tpm_e7472ea12d53461b8d293488f3ed72ec'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'TPM2'
- config_sets:
  - tpm_main_config:
    - tpm_config:
      - clockSource: 'kTPM_SystemClock'
      - tpmSrcClkFreq: 'BOARD_BootClockRUN'
      - prescale: 'kTPM_Prescale_Divide_4'
      - timerFrequency: '2500'
      - useGlobalTimeBase: 'false'
      - triggerSelect: 'kTPM_Trigger_Select_0'
      - triggerSource: 'kTPM_TriggerSource_External'
      - enableDoze: 'false'
      - enableDebugMode: 'false'
      - enableReloadOnTrigger: 'false'
      - enableStopOnOverflow: 'false'
      - enableStartOnTrigger: 'false'
      - enablePauseOnTrigger: 'false'
    - timer_interrupts: ''
    - enable_irq: 'false'
    - tpm_interrupt:
      - IRQn: 'TPM2_IRQn'
      - enable_priority: 'false'
      - enable_custom_name: 'false'
    - EnableTimerInInit: 'true'
  - tpm_edge_aligned_mode:
    - tpm_edge_aligned_channels_config:
      - 0:
        - edge_aligned_mode: 'kTPM_EdgeAlignedPwm'
        - edge_aligned_pwm:
          - chnlNumber: 'kTPM_Chnl_1'
          - level: 'kTPM_HighTrue'
          - dutyCyclePercent: '50'
          - enable_chan_irq: 'false'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const tpm_config_t TPM_2_config = {
  .prescale = kTPM_Prescale_Divide_4,
  .useGlobalTimeBase = false,
  .triggerSelect = kTPM_Trigger_Select_0,
  .triggerSource = kTPM_TriggerSource_External,
  .enableDoze = false,
  .enableDebugMode = false,
  .enableReloadOnTrigger = false,
  .enableStopOnOverflow = false,
  .enableStartOnTrigger = false,
  .enablePauseOnTrigger = false
};

const tpm_chnl_pwm_signal_param_t TPM_2_pwmSignalParams[] = {
  {
    .chnlNumber = kTPM_Chnl_1,
    .level = kTPM_HighTrue,
    .dutyCyclePercent = 0
  }
};

void TPM_2_init(void) {
  TPM_Init(TPM_2_PERIPHERAL, &TPM_2_config);
  TPM_SetupPwm(TPM_2_PERIPHERAL, TPM_2_pwmSignalParams, sizeof(TPM_2_pwmSignalParams) / sizeof(tpm_chnl_pwm_signal_param_t), kTPM_EdgeAlignedPwm, 2800, TPM_2_CLOCK_SOURCE);
  TPM_StartTimer(TPM_2_PERIPHERAL, kTPM_SystemClock);
}

/***********************************************************************************************************************
 * SPI_0 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'SPI_0'
- type: 'spi'
- mode: 'dma'
- type_id: 'spi_672b694426b0a10a1d774659ee8f8435'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'SPI0'
- config_sets:
  - fsl_spi:
    - spi_mode: 'kSPI_Master'
    - clockSource: 'BusInterfaceClock'
    - clockSourceFreq: 'GetFreq'
    - spi_master_config:
      - enableMaster: 'true'
      - enableStopInWaitMode: 'false'
      - polarity: 'kSPI_ClockPolarityActiveHigh'
      - phase: 'kSPI_ClockPhaseFirstEdge'
      - direction: 'kSPI_MsbFirst'
      - dataMode: 'kSPI_8BitMode'
      - outputMode: 'kSPI_SlaveSelectAsGpio'
      - pinMode: 'kSPI_PinModeNormal'
      - baudRate_Bps: '500000'
  - dmaCfg:
    - dma_channels:
      - enable_rx_dma_channel: 'true'
      - dma_rx_channel:
        - DMAn: '0'
        - DMA_source: 'kDmaRequestMux0SPI0Rx'
        - enable_custom_name: 'false'
      - enable_tx_dma_channel: 'true'
      - dma_tx_channel:
        - DMAn: '1'
        - DMA_source: 'kDmaRequestMux0SPI0Tx'
        - enable_custom_name: 'false'
    - spi_dma_handle:
      - enable_custom_name: 'false'
      - init_callback: 'true'
      - callback_fcn: 'SPI_0_masterCallback'
      - user_data: ''
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const spi_master_config_t SPI_0_config = {
  .enableMaster = true,
  .enableStopInWaitMode = false,
  .polarity = kSPI_ClockPolarityActiveHigh,
  .phase = kSPI_ClockPhaseFirstEdge,
  .direction = kSPI_MsbFirst,
  .dataMode = kSPI_8BitMode,
  .outputMode = kSPI_SlaveSelectAsGpio,
  .pinMode = kSPI_PinModeNormal,
  .baudRate_Bps = 500000
};
dma_handle_t SPI_0_RX_Handle;
dma_handle_t SPI_0_TX_Handle;
spi_dma_handle_t SPI_0_DMA_Handle;

void SPI_0_init(void) {
  /* Initialization function */
  SPI_MasterInit(SPI_0_PERIPHERAL, &SPI_0_config, SPI_0_CLK_FREQ);
  /* Set the source kDmaRequestMux0SPI0Rx request in the DMAMUX */
  DMAMUX_SetSource(SPI_0_RX_DMAMUX_BASEADDR, SPI_0_RX_DMA_CHANNEL, SPI_0_RX_DMA_REQUEST);
  /* Enable the 0 channel in the DMAMUX */
  DMAMUX_EnableChannel(SPI_0_RX_DMAMUX_BASEADDR, SPI_0_RX_DMA_CHANNEL);
  /* Set the source kDmaRequestMux0SPI0Tx request in the DMAMUX */
  DMAMUX_SetSource(SPI_0_TX_DMAMUX_BASEADDR, SPI_0_TX_DMA_CHANNEL, SPI_0_TX_DMA_REQUEST);
  /* Enable the 1 channel in the DMAMUX */
  DMAMUX_EnableChannel(SPI_0_TX_DMAMUX_BASEADDR, SPI_0_TX_DMA_CHANNEL);
  /* Create the DMA SPI_0_RX_Handle handle */
  DMA_CreateHandle(&SPI_0_RX_Handle, SPI_0_RX_DMA_BASEADDR, SPI_0_RX_DMA_CHANNEL);
  /* Create the DMA SPI_0_TX_Handle handle */
  DMA_CreateHandle(&SPI_0_TX_Handle, SPI_0_TX_DMA_BASEADDR, SPI_0_TX_DMA_CHANNEL);
  /* Create the SPI DMA handle */
  SPI_MasterTransferCreateHandleDMA(SPI_0_PERIPHERAL, &SPI_0_DMA_Handle, SPI_0_masterCallback, NULL, &SPI_0_TX_Handle, &SPI_0_RX_Handle);
}

/***********************************************************************************************************************
 * I2C_0 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'I2C_0'
- type: 'i2c'
- mode: 'dma'
- type_id: 'i2c_2566d7363e7e9aaedabb432110e372d7'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'I2C0'
- config_sets:
  - fsl_i2c:
    - i2c_mode: 'kI2C_Master'
    - clockSource: 'BusInterfaceClock'
    - clockSourceFreq: 'GetFreq'
    - i2c_master_config:
      - enableMaster: 'true'
      - enableStopHold: 'false'
      - baudRate_Bps: '100000'
      - glitchFilterWidth: '0'
    - quick_selection: 'QS_I2C_1'
  - dmaCfg:
    - dma_channel:
      - enable_dma_channel: 'true'
      - dma_channel:
        - DMAn: '2'
        - DMA_source: 'kDmaRequestMux0I2C0'
        - enable_custom_name: 'false'
    - i2c_dma_handle:
      - enable_custom_name: 'false'
      - init_callback: 'true'
      - callback_fcn: 'i2c_master_callback'
      - user_data: ''
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const i2c_master_config_t I2C_0_config = {
  .enableMaster = true,
  .enableStopHold = false,
  .baudRate_Bps = 600000,
  .glitchFilterWidth = 0
};
dma_handle_t I2C_0_Handle;
i2c_master_dma_handle_t I2C_0_DMA_Handle;

void I2C_0_init(void) {
  /* Initialization function */
  I2C_MasterInit(I2C_0_PERIPHERAL, &I2C_0_config, I2C_0_CLK_FREQ);
  /* Set the source kDmaRequestMux0I2C0 request in the DMAMUX */
  DMAMUX_SetSource(I2C_0_DMAMUX_BASEADDR, I2C_0_DMA_CHANNEL, I2C_0_DMA_REQUEST);
  /* Enable the 2 channel in the DMAMUX */
  DMAMUX_EnableChannel(I2C_0_DMAMUX_BASEADDR, I2C_0_DMA_CHANNEL);
  /* Create the DMA I2C_0_Handle handle */
  DMA_CreateHandle(&I2C_0_Handle, I2C_0_DMA_BASEADDR, I2C_0_DMA_CHANNEL);
  /* Create the I2C DMA handle */
  I2C_MasterTransferCreateHandleDMA(I2C_0_PERIPHERAL, &I2C_0_DMA_Handle, i2c_master_callback, NULL, &I2C_0_Handle);
}

/***********************************************************************************************************************
 * ADC16_1 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'ADC16_1'
- type: 'adc16'
- mode: 'ADC'
- type_id: 'adc16_7d827be2dc433dc756d94a7ce88cbcc5'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'ADC0'
- config_sets:
  - fsl_adc16:
    - adc16_config:
      - referenceVoltageSource: 'kADC16_ReferenceVoltageSourceVref'
      - clockSource: 'kADC16_ClockSourceAsynchronousClock'
      - enableAsynchronousClock: 'true'
      - clockDivider: 'kADC16_ClockDivider8'
      - resolution: 'kADC16_ResolutionSE12Bit'
      - longSampleMode: 'kADC16_LongSampleCycle24'
      - enableHighSpeed: 'false'
      - enableLowPower: 'true'
      - enableContinuousConversion: 'true'
    - adc16_channel_mux_mode: 'kADC16_ChannelMuxA'
    - adc16_hardware_compare_config:
      - hardwareCompareModeEnable: 'false'
    - doAutoCalibration: 'false'
    - offset: '0'
    - trigger: 'false'
    - hardwareAverageConfiguration: 'kADC16_HardwareAverageCount32'
    - enable_dma: 'false'
    - enable_irq: 'false'
    - adc_interrupt:
      - IRQn: 'ADC0_IRQn'
      - enable_priority: 'false'
      - enable_custom_name: 'false'
    - adc16_channels_config:
      - 0:
        - enableDifferentialConversion: 'false'
        - channelNumber: 'SE.23'
        - enableInterruptOnConversionCompleted: 'false'
        - channelGroup: '0'
        - initializeChannel: 'true'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
adc16_channel_config_t ADC0_channelsConfig[1] = {
  {
    .channelNumber = 23U,
    .enableDifferentialConversion = false,
    .enableInterruptOnConversionCompleted = true,
  }
};
const adc16_config_t ADC0_config = {
  .referenceVoltageSource = kADC16_ReferenceVoltageSourceValt,
  .clockSource = kADC16_ClockSourceAlt1,
  .enableAsynchronousClock = false,
  .clockDivider = kADC16_ClockDivider8,
  .resolution = kADC16_ResolutionSE16Bit,
  .longSampleMode = kADC16_LongSampleCycle24,
  .enableHighSpeed = false,
  .enableLowPower = true,
  .enableContinuousConversion = true
};
const adc16_channel_mux_mode_t ADC0_muxMode = kADC16_ChannelMuxA;
const adc16_hardware_average_mode_t ADC0_hardwareAverageMode = kADC16_HardwareAverageCount32;

void ADC0_init(void) {
	/* Enable interrupt ADC0_IRQN request in the NVIC */
	  EnableIRQ(ADC0_IRQN);
	  /* Initialize ADC16 converter */
	  ADC16_Init(ADC0_PERIPHERAL, &ADC0_config);
	  /* Make sure, that software trigger is used */
	  ADC16_EnableHardwareTrigger(ADC0_PERIPHERAL, false);
	  /* Configure hardware average mode */
	  ADC16_SetHardwareAverage(ADC0_PERIPHERAL, ADC0_hardwareAverageMode);
	  /* Configure channel multiplexing mode */
	  ADC16_SetChannelMuxMode(ADC0_PERIPHERAL, ADC0_muxMode);
	  /* Initialize channel */
	  ADC16_SetChannelConfig(ADC0_PERIPHERAL, 0U, &ADC0_channelsConfig[0]);
}

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/
void BOARD_InitPeripherals(void)
{
	/* Global initialization */
	  DMAMUX_Init(DMA_1_DMAMUX_BASEADDR);
	  DMA_Init(DMA_1_DMA_BASEADDR);

	  /* Initialize components */
	  TPM_2_init();
	  SPI_0_init();
	  I2C_0_init();
	  ADC0_init();
}

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void)
{
  BOARD_InitPeripherals();
}
