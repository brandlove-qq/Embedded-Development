#ifndef _SHT20_H_
#define _SHT20_H_

#include "stm32f4xx.h"
extern u8 tem_buffr[5];
extern u8 Rh_buffr[5];
extern float tem;
extern float rh;

void SHT20_Tem(void);
void SHT20_Rh(void);


#endif
