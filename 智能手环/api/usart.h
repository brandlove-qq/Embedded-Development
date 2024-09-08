#ifndef __USART_H
#define __USART_H 

#include "stm32f4xx.h"

void Usart_Init(void);
void Usart_SendByte(u8 dat);
void Usart_Str(u8 *p);
u8 Usart_Receive(void);

	  

#endif	   
















