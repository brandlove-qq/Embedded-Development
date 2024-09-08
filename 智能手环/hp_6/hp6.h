/***********************************************************************************************************************
Copyright 2008 - 2016 深圳市信盈达电子有限公司. All rights reserved.
文件名:        hp6.h
描述   :       心率血压模块驱动文件
作者   :       Jahol Fan
版本   :       V1.0
修改   :   
完成日期：     2016.10.13
Notice    :本程序只供学习使用，未经作者许可，不得用于其它任何用途
信盈达官网：http://www.edu118.com/
信盈达网校：http://www.edu118.cn/
版权所有，盗版必究。
************************************************************************************************************************/


// IIC 速率使用100K；7bite地址：0x66

#ifndef HP_6_PROTOCOL_H
#define HP_6_PROTOCOL_H
#include "stm32f4xx.h"
#define HP_6_FAILURE			0x00
#define HP_6_SUCCESS			0x01
void Hp_6_init(void);
void Hp_6_PowerON(void);
void Hp_6_PowerOFF(void);
uint16_t HP_6_GetResultData(uint8_t *data);

uint8_t HP_6_GetRateResult(void);
uint8_t HP_6_OpenRate(void);
uint8_t HP_6_CloseRate(void);

uint8_t HP_6_GetBpResult(void);
uint8_t HP_6_OpenBp(void);
uint8_t HP_6_CloseBp(void);

uint8_t HP_6_GetADC(uint8_t num, uint8_t id);

uint8_t HP_6_PowerSaving(void);

uint8_t HP_6_VersionInfo(void);

#define Hp_6_PowerON()           GPIOB->ODR &= ~(1<<15)			  //PB15输出低电平
#define Hp_6_PowerOFF()      		 GPIOB->ODR |=  (1<<15)			  //PB15输出高电平


#endif

