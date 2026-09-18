/* flightStateDetector.h
 *
 *  Created on: 19 cze 2018
 *      Author: b.moczala
 */

#ifndef FLIGHTSTATEDETECTOR_H_
#define FLIGHTSTATEDETECTOR_H_
#include <stdint.h>
#include "struct.h"

#define TRUE 1
#define FALSE 0

typedef enum {
	WAIT_FOR_LAUNCH,
	ASCENT,
	FALLING,
	LANDING
} FlightStateList_t;

typedef struct {
	FlightStateList_t state;
	uint8_t state_ready;
} flightState_t;




void FlightState_Init(sensors_t * val);
uint8_t FlightState_getState();
void FlightState_forceState(uint8_t new_state);
void FlightState_DelayInit(uint32_t delay_ms);
uint8_t FlightState_DelayCheck();

void FlightState_Detect(uint32_t time);

#endif /* FLIGHTSTATEDETECTOR_H_ */

