/*
 * buzzer.c
 *
 *  Created on: 27 mar 2019
 *      Author: b.moczala
 */

#include "fsl_common.h"
#include "peripherals.h"
#include "buzzer.h"

#define BUZZER_DUTY		18

typedef enum {
	BUZZER_IDLE = 0,
	BUZZER_TONE,
	BUZZER_GAP
} buzzer_phase_t;

static volatile buzzer_phase_t phase = BUZZER_IDLE;
static volatile uint16_t ton_ms;
static volatile uint16_t toff_ms;
static volatile uint16_t remain;
static volatile uint16_t beeps_left;
static volatile uint8_t infinite;

static void Buzzer_hwOn(void){
	TPM_UpdatePwmDutycycle(TPM2_PERIPHERAL, 1, 0, BUZZER_DUTY);
}

static void Buzzer_hwOff(void){
	TPM_UpdatePwmDutycycle(TPM2_PERIPHERAL, 1, 0, 0);
}

static void Buzzer_startTone(void){
	phase = BUZZER_TONE;
	remain = ton_ms;
	Buzzer_hwOn();
}

void Buzzer_stop(void){
	uint32_t primask = DisableGlobalIRQ();
	phase = BUZZER_IDLE;
	remain = 0;
	beeps_left = 0;
	infinite = 0;
	Buzzer_hwOff();
	EnableGlobalIRQ(primask);
}

void Buzzer_pulse(uint16_t count, uint16_t t_on, uint16_t t_off, uint8_t inf){
	if((t_on == 0) || (!inf && (count == 0))){
		Buzzer_stop();
		return;
	}

	uint32_t primask = DisableGlobalIRQ();
	ton_ms = t_on;
	toff_ms = t_off;
	infinite = inf ? 1 : 0;
	beeps_left = inf ? 0 : count;
	Buzzer_startTone();
	EnableGlobalIRQ(primask);
}

void Buzzer_shortBeep(void){
	Buzzer_pulse(1, BUZZER_SHORT_MS, 0, 0);
}

void Buzzer_longBeep(void){
	Buzzer_pulse(1, BUZZER_LONG_MS, 0, 0);
}

void Buzzer_service(void){
	if(phase == BUZZER_IDLE)
		return;

	if(remain > 0)
		remain--;
	if(remain > 0)
		return;

	if(phase == BUZZER_TONE){
		Buzzer_hwOff();
		if(!infinite){
			if(beeps_left)
				beeps_left--;
			if(beeps_left == 0){
				phase = BUZZER_IDLE;
				return;
			}
		}
		if(toff_ms == 0){
			Buzzer_startTone();
			return;
		}
		phase = BUZZER_GAP;
		remain = toff_ms;
	} else {
		Buzzer_startTone();
	}
}
