/*
 * buzzer.c
 *
 *  Created on: 27 mar 2019
 *      Author: b.moczala
 */

#include "peripherals.h"
#include "buzzer.h"


void Buzzer_ON(){
	TPM_UpdatePwmDutycycle(TPM_2_PERIPHERAL, 1, 0, 18);
}

void Buzzer_OFF(){
	TPM_UpdatePwmDutycycle(TPM_2_PERIPHERAL, 1, 0, 0);
}
