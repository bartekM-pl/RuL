/*
 * Copyright 2016-2018 NXP
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
 
/**
 * @file    I_PRV_RuL_1_1.c
 * @brief   Application entry point.
 */
#include <flightStateDetector.h>
#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "fsl_gpio.h"

#include "utils.h"
#include "spi.h"
#include "i2c.h"
#include "oled.h"
#include "flash.h"
#include "buzzer.h"
#include "LPS25HB.h"
#include "LIS.h"
#include "PSU.h"
#include "analog.h"
#include "FS.h"
#include "struct.h"
#include "MKL27Z4.h"

sensors_t sensors_d;
stat_t status_d;
max_t max_d;

volatile uint32_t Clock = 0;
volatile uint8_t counter_meas = 0;
volatile uint8_t counter_log  = 0;

volatile uint8_t button_cnt = 0;
volatile uint8_t button_idle_cnt = 0;

uint32_t meas_period = 100;
uint32_t log_period  = 100;

void SysTick_Handler(){
	TimingDelay_Decrement();
	counter_meas++;
	counter_log++;
	Clock++;

	if(!(Clock % 100)){
		if(Button_state()){
			button_cnt++;
			button_idle_cnt = 0;
		}
		else if(button_idle_cnt < 100)
			button_idle_cnt++;

		if(button_cnt > 20){	//przytrzymanie 2s -> wyłączenie
			button_cnt = 0;
			LED_blue(1);
			//OLED_clear(0);
			//OLED_refresh();

			while(Button_state()) {}
			PSU_turnOff();
		}
		else if((button_cnt >= 2) && !Button_state()){	//przyciśnięcie >= 300ms i puszczenie
			button_cnt = 0;
			LED_blue(1);
			//OLED_nextScreen();
		}
		else {
			LED_blue(0);
			//button_cnt = 0;
		}
	}

	// Uśpienie ekranu po 5s
	if(button_idle_cnt > 50){
		//OLED_sleepScreen();
	}

	//co 250ms rysuj OLED
	if(!(Clock % 250)){
		//OLED_render(&status_d, &sensors_d, &max_d);
	}

	//Heartbeat LED
	if(!(Clock % 3000) || !((Clock - 200) % 3000))
		LED_green(1);
	if(!((Clock - 25) % 3000) || !((Clock - 225) % 3000))
		LED_green(0);
}

void ADC0_IRQHandler(){
    Analog_setVbatRaw(ADC16_GetChannelConversionValue(ADC0, 0U));
    status_d.vbat = Analog_getVbat();
    status_d.memfree = 100.0f;
}


/*
 * @brief   Application entry point.
 */
int main(void) {
  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    SysTick_Config(CLOCK_GetFreq(kCLOCK_CoreSysClk) / 1000U);
    PSU_turnOn();
    //printf("MCU ready\n");


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

    uint16_t freq = 1000;

    while(1) {
			Buzzer_ON();
			delay(300);

    }
    return 0 ;
}
