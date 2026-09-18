/*
 * analog.c
 *
 *  Created on: 4 maj 2019
 *      Author: bartek
 */


#include <stdio.h>
#include <stdlib.h>
#include "MKL27Z4.h"
#include "analog.h"

uint32_t VbatRaw = 0;
float Vbat = 0.0f;

float Analog_getVbat(){
	return Vbat;
}

void Analog_setVbatRaw(uint32_t val){
	VbatRaw = val;
	Vbat = 2.0f * (float)val * 3.0f / 65536.0f;	//2x bo dzielnik 1:2,   3.0f bo 3V Vref,    65536.0f bo 16bit
}
