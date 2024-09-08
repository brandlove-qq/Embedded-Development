#ifndef _MPU6050_H_
#define _MPU6050_H_
#include "stm32f4xx.h"
#include "delay.h"
#include "iic.h"
#include "oled.h"

extern u16 MPU_Value_AX;
extern u16 MPU_Value_AY;
extern u16 MPU_Value_AZ;
extern u16 MPU_Value_GX;
extern u16 MPU_Value_GY;
extern u16 MPU_Value_GZ;

typedef struct 
{
	float accX;
	float accY;
	float accZ;
}accValue_t;

typedef	struct
{
	u8 hour;
	u8 minute;
	u8 second;
	u8 twentyMsCount;
}timeStamp_t;

typedef struct 
{
	float calories;
	float distance;
	u32 stepCount;	
}walk_t;


#define MPU6050_IIC_ADDRESS 0x68
#define MPU6050_Writ_ADDR 0xD0 //(0x68<<1) | 0 
#define MPU6050_Read_ADDR 0xD1 //(0x68<<1) | 1

u8 MPU6050_Init(void);
u8 MPU_Read_Data(void);
void MPU6050_Init_Check(void);
void onSensorChanged(accValue_t *pAccValue,timeStamp_t *timeStamp_p);
void DetectorNewStep(float values,timeStamp_t *timeStam_p) ;
u8 DetectorPeak(float newValue, float oldValue) ;
float Peak_Valley_Thread(float value);
float averageValue(float value[], int n);



#endif
