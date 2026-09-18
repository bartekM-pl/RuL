/*
 * utils.h
 *
 *  Created on: 22 mar 2019
 *      Author: bartek
 */

#ifndef UTILS_H_
#define UTILS_H_

void TimingDelay_Decrement();
void delay(__IO uint32_t nTime);
void LED_red(uint8_t val);
void LED_green(uint8_t val);
void LED_blue(uint8_t val);
uint8_t Button_state();
void PowerSwitch(uint8_t val);

#endif /* UTILS_H_ */
