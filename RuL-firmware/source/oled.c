/*
 * oled.c
 *
 *  Created on: 22 mar 2019
 *      Author: bartek
 */


#include <stdio.h>
#include <stdlib.h>
#include "MKL27Z4.h"
#include "struct.h"
#include "flightStateDetector.h"
#include "fonts.h"
#include "i2c.h"
#include "oled.h"

void OLED_sleep();
void OLED_wakeup();

OLED_t OLED_d;
OLED_interface_t OLED_interface_d;

//==========================================================================================================
//                              OLED
//==========================================================================================================

void OLED_init(){
	OLED_interface_d.screen = 0;

	OLED_sendCommand(0xAE);//wy³¹cz panel OLED
	//OLED_sendCommand(0x00);//adres kolumny LOW
	//OLED_sendCommand(0x10);//adres kolumny HIGH
	//OLED_sendCommand(0x40);//adres startu linii

	OLED_sendCommand(0x20);//tryb adresowania strony
	OLED_sendCommand(0x02);

	OLED_sendCommand(0x81);//ustaw kontrast
	OLED_sendCommand(1);

	OLED_sendCommand(0xA0);//ustaw remapowanie    //by³o A1
	OLED_sendCommand(0xC8);//kierunek skanowania //by³o C0


	OLED_sendCommand(0xA8);//ustaw multiplex ratio
	OLED_sendCommand(31);//1/64

	OLED_sendCommand(0xA6);//wyœwietlanie bez inwersji
	OLED_sendCommand(0xD3);//ustaw display offset
	OLED_sendCommand(0x00);//bez offsetu
	OLED_sendCommand(0xD5);//ustaw divide ratio/czêstotliwoœæ oscylatora
	OLED_sendCommand(0x80);//100ramek/sec
	OLED_sendCommand(0xD9);//ustaw okres pre charge
	OLED_sendCommand(0xF1);//pre charge 15 cykli, discharge 1 cykl
	OLED_sendCommand(0xDA);//konfiguracja wyprowadzeñ sterownika
	OLED_sendCommand(0x12);
	OLED_sendCommand(0xDB);//ustawienie vcomh
	OLED_sendCommand(0x40);
	OLED_sendCommand(0x8D);//ustawienie Charge Pump
	OLED_sendCommand(0x14);
	OLED_sendCommand(0xA4);//"pod³¹czenie" zawartoœci RAM do panelu OLED
	OLED_sendCommand(0xA6);//wy³¹czenie inwersji wyœwietlania
	OLED_sendCommand(0xAF);//w³¹cza wyœwietlacz
}

void OLED_sleep(){
	OLED_sendCommand(0xAE);//wylacz panel OLED
}

void OLED_wakeup(){
	OLED_sendCommand(0xAF);//wlacz panel OLED
}

void OLED_sendCommand(uint8_t val){
  I2C_Send(0x3C, 0x00, 1, &val, 1);
}

void OLED_sendCommandData(uint8_t cmd, uint8_t dat){
	uint8_t buf[2] = {cmd, dat};
  I2C_Send(0x3C, 0x00, 1, buf, 2);
}

void OLED_sendData(uint8_t val){
  I2C_Send(0x3C, 0x40, 1, &val, 1);
}

void OLED_refresh(){
  uint8_t i, j;

  for (i = 0; i < 4; i ++) {
    OLED_sendCommand(0xB0 + i);
    OLED_setColStart();
    for (j = 0; j < 96; j ++) {
      OLED_sendData(OLED_d.OLED_dispBuff[j][i]);
    }
  }
}

void OLED_clear(uint8_t fill){
  uint8_t i, j;

  for (i = 0; i < 4; i ++) {
    for (j = 0; j < 96; j ++) {
      OLED_d.OLED_dispBuff[j][i] = fill;
    }
  }

  //OLED_RefreshRAM(OLED);//zawartoœæ bufora do RAM obrazu
}

void OLED_setColStart(){
    OLED_sendCommand(0x00); //low
    OLED_sendCommand(0x12); //high
}


void OLED_drawPoint(uint8_t x, uint8_t y, uint8_t p){
  uint8_t chPos, chBx, chTemp = 0;

  if (x > 95 || y > 31) {
    return;
  }
  chPos = (31-y) / 8;
  chBx = (31-y) % 8;
  chTemp = 1 << (chBx);

  if (p) {
    OLED_d.OLED_dispBuff[95-x][chPos] |= chTemp;

  } else {
	OLED_d.OLED_dispBuff[95-x][chPos] &= ~chTemp;
  }
}

//wyœwietlenie jednego znaku
//argumenty:
//x,y - wspó³rzêdne na ekranie
//Chr - kod ASCII znaku
//size - rozmiar 12, lub 16
//mode=1 znak wyœwietlany normalnie, mode=0 znak wyœwietlany w negatywie
//*******************************************************************************
void OLED_displayChar(uint8_t x, uint8_t y, uint8_t Chr){
  uint8_t i, j;
  uint8_t chTemp, chYpos0 = y;

  Chr = Chr - ' ' + 1;
  //Chr = OLED_charDecoder(Chr);
  for (i = 0; i < 12; i ++) {
        chTemp = c_chFont1206[Chr][i];

    for (j = 0; j < 8; j ++) {
      if (chTemp & 0x80) {
        OLED_drawPoint(x, y, 1);
      } else {
        OLED_drawPoint(x, y, 0);
      }
      chTemp <<= 1;
      y ++;

      if ((y - chYpos0) == 12) {
        y = chYpos0;
        x ++;
        break;
      }
    }
  }
}

uint8_t OLED_charDecoder(uint8_t znak){
  /*
  if((znak >= '0') && (znak <= '9')) return znak-41;
  switch(znak){
    case '!': return 1;
    case '%': return 2;
    case '+': return 3;
    case '-': return 4;
    case '.': return 5;
    case '/': return 6;
    case ':': return 17;
    case 'A': return 18;
    case 'H': return 19;
    case 'P': return 20;
    case 'V': return 21;
    case '^': return 22;
    case 'a': return 23;
    case 'm': return 24;
    case 'x': return 25;
    case 's': return 26;
    case ' ': return 0;
  }
*/
  return font_decoder[znak];



  //return 0;
}

//*********************************************************************************
//wyœwietlenie ³añcucha znaków - napisu
//argumenty
//x,y - wspó³rzêdne na ekranie
//*txt - wskaŸnik na pocz¹tek bufora zwieraj¹cego ³añcuch znaków ASCII do wyœwietlenia
//size - wysokoœæ znaków 12, lub 16 pikseli
//mode=1 znaki wyœwietlane normalnie, mode=0 znaki wyœwietlane w negatywie
//*********************************************************************************
void OLED_dispTxt(uint8_t x, uint8_t y,  char *txt){
  while (*txt != '\0') {

    OLED_displayChar(x, y, *txt);
    x += 6;
    txt ++;
  }
}

void OLED_int2string(uint8_t * string, int32_t number){
  sprintf((char*)string, "%+6.1f", (float)number/10.0);
}

void OLED_float2string(uint8_t * string, float number){
	sprintf((char*)string, "%+6.1f", number);
}

void OLED_measTemplate(){
  OLED_dispTxt(0, 0,"Vmax:       m/s ");
  OLED_dispTxt(0,10,"Amax:       m/s^");
  OLED_dispTxt(0,21,"Hmax:       m   ");
}

void OLED_statusTemplate(){
  OLED_dispTxt(0, 0,"Vbat:        V  ");
  OLED_dispTxt(0,10,"Press:       hPa");
  OLED_dispTxt(0,21,"Flash:       %  ");
}

void OLED_dispInt(uint8_t x, uint8_t y, int32_t value){
  uint8_t bufor[7];
  OLED_int2string(bufor, value);

  OLED_dispTxt(x, y, (char*)bufor);
}

void OLED_dispVbat(float value){
	char buf[10];
	sprintf(buf, "%3li", (uint32_t)(value*100));
	buf[4] = 0;
	buf[3] = buf[2];
	buf[2] = buf[1];
	buf[1] = '.';
	OLED_dispTxt(53, 0, buf);
}

void OLED_dispPress(float value){
	char buf[10];
	sprintf(buf, "%5li", (uint32_t)(value/10));
	buf[6] = 0;
	buf[5] = buf[4];
	buf[4] = '.';
	OLED_dispTxt(40, 10, buf);
}

void OLED_dispFlash(float value){
	char buf[10];
	sprintf(buf, "%3li", (uint32_t)(value));
	OLED_dispTxt( 58, 21, buf);
}

void OLED_dispVelocity(float value){
	value = (value>=0)?value:(-value);
	char buf[10];
	sprintf(buf, "%4li", (uint32_t)(value*10.0));
	buf[5] = 0;
	buf[4] = buf[3];
	buf[3] = '.';
	if(value<1) buf[2] = '0';
	OLED_dispTxt(40, 0, buf);
}

void OLED_dispAcceleration(float value){
	value = (value>=0)?value:(-value);
	char buf[10];
	sprintf(buf, "%4li", (uint32_t)(value*10.0));
	buf[5] = 0;
	buf[4] = buf[3];
	buf[3] = '.';
	if(value<1) buf[2] = '0';
	OLED_dispTxt(40, 10, buf);
}

void OLED_dispAltitude(float value){
	char buf[10];
	sprintf(buf, "%6li", (uint32_t)(value));
	OLED_dispTxt(34, 21, buf);
}


void OLED_drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode){
  uint8_t tmp;
  uint8_t x,y;
  uint8_t dx, dy;
  int8_t err;
  int8_t ystep;

  uint8_t swapxy = 0;

  if ( x1 > x2 ) dx = x1-x2; else dx = x2-x1;
  if ( y1 > y2 ) dy = y1-y2; else dy = y2-y1;

  if ( dy > dx )
  {
    swapxy = 1;
    tmp = dx; dx =dy; dy = tmp;
    tmp = x1; x1 =y1; y1 = tmp;
    tmp = x2; x2 =y2; y2 = tmp;
  }
  if ( x1 > x2 )
  {
    tmp = x1; x1 =x2; x2 = tmp;
    tmp = y1; y1 =y2; y2 = tmp;
  }
  err = dx >> 1;
  if ( y2 > y1 ) ystep = 1; else ystep = -1;
  y = y1;

  if ( x2 == 255 ) x2--;

  for( x = x1; x <= x2; x++ ){
    if ( swapxy == 0 )
      OLED_drawPoint(x, y, mode);
    else
      OLED_drawPoint(y, x, mode);
    err -= (uint8_t)dy;
    if ( err < 0 ){
      y += (uint8_t)ystep;
      err += (uint8_t)dx;
    }
  }
}



void OLED_drawPlotTemplate(char type){
  //----  X axis  ---------
  OLED_drawLine(0, 29, 90, 29, 1);
  OLED_drawLine(90, 29, 88, 27, 1);
  OLED_drawLine(90, 29, 88, 31, 1);
  OLED_displayChar(91, 20, 't');

  //---- Y axis  ----------
  OLED_drawLine(2, 6, 2, 31, 1);
  OLED_drawLine(2, 6, 0,  8, 1);
  OLED_drawLine(2, 6, 4,  8, 1);
  OLED_displayChar(4, 0, type);
}


//-------------------------- OLED tasks ----------------------------------------
void OLED_render(stat_t * stat, sensors_t * meas, max_t * max){
	switch(OLED_interface_d.screen){
	case 0:
		OLED_renderStatus(stat, meas);
		break;

	case 1:
		OLED_renderMeas(max);
		break;

	case 2:
		OLED_turnOff();
		break;

	default:
		break;
	}
	OLED_refresh();
}

void OLED_nextScreen(){
	if(OLED_interface_d.screen == 2)	//wakeup from sleep
		OLED_wakeup();

	OLED_interface_d.screen++;
	if(OLED_interface_d.screen >= 3)
		OLED_interface_d.screen = 0;
}

void OLED_sleepScreen(){
	OLED_interface_d.screen = 2;
}

void OLED_renderStatus(stat_t * stat, sensors_t * meas){
	OLED_statusTemplate();
	OLED_dispVbat(stat->vbat);
	OLED_dispPress(meas->pressure);
	OLED_dispFlash(stat->memfree);
}

void OLED_renderMeas(max_t * max){
	OLED_measTemplate();
	OLED_dispVelocity(max->velocity);
	OLED_dispAcceleration(max->acc);
	OLED_dispAltitude(max->altitude);
}

void OLED_turnOff(){
	OLED_clear(0);			//TODO: ------------------------------------------------Dopisać wyłącznie wyświetlacza LP
	OLED_sleep();
}

//==========================================================================================================
//                              Dev
//==========================================================================================================
/*
void dev_CheckSensors(){
  OLED_Clear(&OLED_buffer, 0);

  if(I2C_ReadOneByte(0xEE, 0xD0) == 0x58) OLED_dispTxt(&OLED_buffer,0,0,"BMP  OK");
  else OLED_dispTxt(&OLED_buffer,0,0,"BMP  NOPE");

  if(I2C_ReadOneByte(0x3A, 0x00) == 0xE5) OLED_dispTxt(&OLED_buffer,0,12,"ADXL OK");
  else OLED_dispTxt(&OLED_buffer,0,12,"ADXL  NOPE");

  OLED_RefreshRAM(&OLED_buffer);
}
*/
