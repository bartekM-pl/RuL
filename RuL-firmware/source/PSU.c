/*
 * PSU.c
 *
 *  Created on: 30 kwi 2019
 *      Author: b.moczala
 */

#include <stdio.h>
#include <stdlib.h>
#include "MKL27Z4.h"
#include "utils.h"
#include "struct.h"
#include "PSU.h"

void PSU_turnOn(){
	PowerSwitch(1);
}

void PSU_turnOff(){
	PowerSwitch(0);
}

void PSU_lowPowerMode(){

}

void PSU_highPowerMode(){

}
