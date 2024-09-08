#ifndef _RTC_H_
#define _RTC_H_

#include "stm32f4xx.h"

u8 RTC_Wait_Synchro(void);
u8 RTC_Init_Mode(void);

void RTC_Write_BKR(u32 BKRx,u32 data);
u32 RTC_Read_BKR(u32 BKRx);

u8 RTC_DEC2BCD(u8 val);
u8 RTC_BCD2DEC(u8 val);
u8 RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);
u8 RTC_Set_Date(u8 year,u8 month,u8 date,u8 week);
void RTC_Get_Time(u8 *hour,u8 *min,u8 *sec,u8 *ampm);
void RTC_Get_Date(u8 *year,u8 *month,u8 *date,u8 *week);
u8 RTC_Init(void);
void RTC_Set_AlarmA(u8 week,u8 hour,u8 min,u8 sec);
void RTC_Set_WakeUp(u8 wksel,u16 cnt);
u8 RTC_Get_Week(u16 year,u8 month,u8 day);


#endif
