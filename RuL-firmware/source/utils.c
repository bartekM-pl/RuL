/*
 * utils.c
 *
 *  Created on: 22 mar 2019
 *      Author: bartek
 */


#include <stdio.h>
#include <stdlib.h>
#include "fsl_gpio.h"
#include "MKL27Z4.h"
#include "utils.h"



static __IO uint32_t TimingDelay;

void TimingDelay_Decrement() {
  if (TimingDelay != 0x00) {
    TimingDelay--;
  }
}

void delay(__IO uint32_t nTime) {
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void LED_red(uint8_t val){
	GPIO_PinWrite(GPIOD, 5, val?0:1);
}

void LED_green(uint8_t val){
	GPIO_PinWrite(GPIOD, 7, val?0:1);
}

void LED_blue(uint8_t val){
	GPIO_PinWrite(GPIOD, 6, val?0:1);
}

uint8_t Button_state(){
	return GPIO_PinRead(GPIOA, 1);
}

void PowerSwitch(uint8_t val){
	GPIO_PinWrite(GPIOE, 0, val?1:0);
}
