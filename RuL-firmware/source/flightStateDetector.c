/*
 * flightStateDetector.c
 *
 *  Created on: 19 cze 2018
 *      Author: b.moczala
 */

#include <stdio.h>
#include <stdlib.h>
#include "MKL27Z4.h"
#include "utils.h"
#include "struct.h"
#include "flightStateDetector.h"


//------ Private fun -----
void FlightState_WAIT_FOR_LAUNCH (uint32_t time, flightState_t * currentState, sensors_t * val);
void FlightState_ASCENT			 (uint32_t time, flightState_t * currentState, sensors_t * val);
void FlightState_FALLING		 (uint32_t time, flightState_t * currentState, sensors_t * val);
void FlightState_LANDING		 (uint32_t time, flightState_t * currentState, sensors_t * val);

//----- Private var -----
static flightState_t flightState_d;
static sensors_t * sensors_ptr;

uint32_t fsdEventTime;



void FlightState_Init(sensors_t * val) {
	sensors_ptr = val;
	flightState_d.state = WAIT_FOR_LAUNCH;
}

uint8_t FlightState_getState(){
	return flightState_d.state;
}

void FlightState_forceState(uint8_t new_state){
	flightState_d.state = new_state;
}

void FlightState_Detect(uint32_t time){
	flightState_t * currentState = &flightState_d;
	sensors_t * sens = sensors_ptr;

	switch(flightState_d.state){
	case WAIT_FOR_LAUNCH:
		FlightState_WAIT_FOR_LAUNCH(time, currentState, sens);
		break;

	case ASCENT:
		FlightState_ASCENT(time, currentState, sens);
		break;

	case FALLING:
		FlightState_FALLING(time, currentState, sens);
		break;

	case LANDING:
		FlightState_LANDING(time, currentState, sens);
		break;

	default:
		break;
	}
}

//=================================================================================================
//								Startup
//					inicjalizacja sprzętu i algorytmów
//=================================================================================================
void FlightState_WAIT_FOR_LAUNCH(uint32_t time, flightState_t * currentState, sensors_t * val) {
	if(!(currentState->state_ready)) {
		currentState->state_ready = TRUE;
		//komendy wykonywane tylko raz
		fsdEventTime = time;
	}

	//********** Warunki przejścia dalej **********
	if (((time - 3000) >= fsdEventTime) && (1)) {
		currentState->state = ASCENT;
		currentState->state_ready = FALSE;
	}
}

//=================================================================================================
//								Internal Check
//			sprawdzenie poprawności działania czujników i aktuatorów
//=================================================================================================
void FlightState_ASCENT(uint32_t time, flightState_t * currentState, sensors_t * val) {
	if(!(currentState->state_ready)) {
		currentState->state_ready = TRUE;
		//komendy wykonywane tylko raz
		fsdEventTime = time;
	}

	//********** Warunki przejścia dalej **********
	if(((time - 3000) >= fsdEventTime) && (1)){
		currentState->state = FALLING;
		currentState->state_ready = FALSE;
	}
}

//=================================================================================================
//								Sensors Calibration
//
//=================================================================================================
void FlightState_FALLING(uint32_t time, flightState_t * currentState, sensors_t * val) {
	if(!(currentState->state_ready)) {
		currentState->state_ready = TRUE;
		//komendy wykonywane tylko raz
		//Kalibracja czujników Gyro, Accel SensorCalibrationStart();
		fsdEventTime = time;
	}

	//********** Warunki przejścia dalej **********
	if(((time - 3000) >= fsdEventTime) && (1)){
		currentState->state = LANDING;
		currentState->state_ready = FALSE;
	}
}

//=================================================================================================
//								Landing
//						Stan po wylądowaniu
//=================================================================================================
void FlightState_LANDING(uint32_t time, flightState_t * currentState, sensors_t * val) {
	if(!(currentState->state_ready)) {
		currentState->state_ready = TRUE;
		//komendy wykonywane tylko raz
	}
}
