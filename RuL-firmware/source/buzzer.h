/*
 * buzzer.h
 *
 *  Created on: 27 mar 2019
 *      Author: b.moczala
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include <stdint.h>

#define BUZZER_SHORT_MS		80
#define BUZZER_LONG_MS		400

/* Call from SysTick every 1 ms. */
void Buzzer_service(void);

void Buzzer_shortBeep(void);
void Buzzer_longBeep(void);
/* count ignored when infinite != 0; runs until Buzzer_stop(). */
void Buzzer_pulse(uint16_t count, uint16_t ton_ms, uint16_t toff_ms, uint8_t infinite);
void Buzzer_stop(void);

#endif /* BUZZER_H_ */
