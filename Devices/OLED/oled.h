#ifndef __OLED_H_
#define __OLED_H_

#include <STC89C5xRC.H>
#include "oledfont.h"
#include "type.h"

//I2C管脚定义
sbit OLED_SCL = P0^0;   //D0（SCLK） 时钟
sbit OLED_SDIN = P0^1;  //D1（MOSI） 数据
 
#define OLED_SCLK_Clr() OLED_SCL  = 0
#define OLED_SCLK_Set() OLED_SCL  = 1
#define OLED_SDIN_Clr() OLED_SDIN = 0
#define OLED_SDIN_Set() OLED_SDIN = 1

#define Max_Column	128
#define Max_Row		  64

typedef enum {
  OLED_CMD = 0,
  OLED_DATA
} OLED_MODE;

typedef enum {
  S6x8 = 0,
  S8x16
} CHAR_SIZE;

void OLED_Clear(void);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, CHAR_SIZE _size);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, CHAR_SIZE _size);
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t no);
void OLED_Init(void);

#endif
