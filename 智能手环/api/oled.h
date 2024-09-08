#ifndef _OLED_H_
#define _OLED_H_

#include "stm32f4xx.h"

extern volatile uint64_t local_time;

#define OLED_CS(x) x?(GPIOB->ODR|=(1<<7)):(GPIOB->ODR&=~(1<<7))
#define OLED_DC(x) x?(GPIOA->ODR|=(1<<15)):(GPIOA->ODR&=~(1<<15))
#define OLED_RES(x) x?(GPIOB->ODR|=(1<<13)):(GPIOB->ODR&=~(1<<13))
void Oled_Init(void);
u8 SPI1_Receive_SendByte(u8 dat);
void OLED_SendCmd(u8 dat);
void OLED_SendData(u8 dat);
void OLED_Init(void);
void OLED_Set_xy(u8 x,u8 y);
void OLED_Clear(void);
void Display_Byte(u8 x,u8 y,const u8 *b);
void Display_CH(u8 x,u8 y,const u8 *ch);
void Display_String(u8 x,u8 y,unsigned char *str);
void Print_BMP(u8 x,u8 y,u8 wight,u8 hight,const unsigned char *bmp);


#endif
