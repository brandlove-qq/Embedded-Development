#include "delay.h"


uint64_t sys_time = 0;

void Delay_1us(void)
{
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();\
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
}

/*******************
���ܣ�΢��
********************/
void Delay_us(u32 time)
{
	while(time--) Delay_1us();
}


//void Delay_us(u32 time)
//{
//	uint64_t tim = local_time;
//	while((local_time - tim)<time);
//}

/************************
���ܣ�����
*************************/
void Delay_ms(u32 time)
{
	uint64_t tim = sys_time;
	while((sys_time - tim)<time);
}

/******************
���ܣ���
*******************/
void Delay_s(u32 time)
{
	uint64_t tim = sys_time;
	while((sys_time - tim)<(1000*time));
}

