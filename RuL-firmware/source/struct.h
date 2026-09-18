/*
 * struct.h
 *
 *  Created on: 31 mar 2019
 *      Author: bartek
 */

#ifndef STRUCT_H_
#define STRUCT_H_

typedef struct{
	float x;
	float y;
	float z;
} vector_t;

typedef struct{
	int16_t x;
	int16_t y;
	int16_t z;
} vector_I16_t;

typedef union {
	uint8_t array[32];
	struct{
		uint32_t time;
		vector_t acc;
		float vel;
		float pressure;
		float altitude;
		float temp;
	};
} sensors_t;

//typedef struct{
//	uint16_t pageNo;
//	uint8_t position;
//	union{
//		FLASH_dataStruct_t FLASH_dataStruct[8];
//		uint8_t data[512];
//		};
//}FLASH_pageStruct_t;


typedef struct{
	float altitude;
	float velocity;
	float acc;
} max_t;

typedef struct{
	float vbat;
	float memfree;
} stat_t;

typedef struct{
	uint8_t screen;
} OLED_interface_t;

typedef union{
	uint8_t val8[4];
	uint32_t val32;
} U32_8_t;

#endif /* STRUCT_H_ */
