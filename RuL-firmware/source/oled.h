/*
 * oled.h
 *
 *  Created on: 22 mar 2019
 *      Author: bartek
 */

#ifndef OLED_H_
#define OLED_H_

#include "struct.h"

typedef struct OLED_s{
  uint8_t OLED_dispBuff[96][4];
} OLED_t;

void OLED_init();
void OLED_sendCommand(uint8_t data);
void OLED_sendCommandData(uint8_t cmd, uint8_t dat);
void OLED_sendData(uint8_t data);
void OLED_refresh();
void OLED_clear(uint8_t fill);
void OLED_setColStart(void);
void OLED_drawPoint(uint8_t x, uint8_t y, uint8_t p);
void OLED_displayChar(uint8_t x, uint8_t y, uint8_t Chr);
void OLED_dispTxt(uint8_t x, uint8_t y, char *txt);
void OLED_setContrast(uint8_t value);
void OLED_int2string(uint8_t * string, int32_t number);
void OLED_measTemplate();
void OLED_statusTemplate();
void OLED_dispInt(uint8_t x, uint8_t y, int32_t value);

void OLED_dispVbat(float value);
void OLED_dispPress(float value);
void OLED_dispFlash(float value);

void OLED_drawLine(uint8_t x1, uint8_t y01, uint8_t x12, uint8_t y2, uint8_t mode);
void OLED_drawPlotTemplate(char type);
uint8_t OLED_charDecoder(uint8_t znak);

void OLED_render(stat_t * stat, sensors_t * meas, max_t * max);
void OLED_nextScreen();
void OLED_sleepScreen();
void OLED_renderStatus(stat_t * stat, sensors_t * meas);
void OLED_renderMeas(max_t * max);
void OLED_turnOff();

#endif /* OLED_H_ */
