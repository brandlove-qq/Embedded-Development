#include "mpu6050.h"
#include "rtc.h"
#include "math.h"

void MPU6050_Init_Check(void)
{
	if(MPU6050_Init() == 0)
	{	
		Display_String(107,0,"Yes");
	}
	else Display_String(107,0,"No");
}

u8 MPU6050_Init(void)
{
	u8 res = 0;
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);//���͵�ַ��д��
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x6B);//��Դ����Ĵ���1
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x80);//��λMPU6050
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_stop();
	Delay_ms(100);
	
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);//���͵�ַ��д��
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x6B);//��Դ����Ĵ���1
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x00);//����MPU6050 
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_stop();
	Delay_ms(1);
	
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);//���͵�ַ��д��
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x1C);//���ٶȼ����üĴ���
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x18);//�����Ǵ�����,��2000dps
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_stop();
	Delay_ms(1);
	
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);//���͵�ַ��д��
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x1C);//���ٶȼ����üĴ���
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x00);//���ٶȴ�����,��2g
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_stop();
	Delay_ms(1);
	
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);//���͵�ַ��д��
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x38);//�ж�ʹ�ܼĴ���
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x00);//�ر������ж�
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_stop();
	Delay_ms(1);
	
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);//���͵�ַ��д��
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x19);//����Ƶ�ʷ�Ƶ��
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x14);//���ò�����50Hz
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_stop();
	Delay_ms(1);
	
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x6A);//�û����ƼĴ���
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x00);//I2C��ģʽ�ر�
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_stop();
	Delay_ms(1);
	
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x23);//FIFOʹ�ܼĴ���
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x00);//�ر�FIFO
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_stop();
	Delay_ms(1);
	
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x37);//�ж�/��·���üĴ���
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x80);//INT���ŵ͵�ƽ��Ч
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_stop();
	Delay_ms(1);
	/*********��֤����ID*******/
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x75);////����ID�Ĵ���
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_start();
	IIC_sendByte(MPU6050_Read_ADDR);//������
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	res = IIC_readByte(1);//��ȡ����,����nACK 
	IIC_stop ();
	
	if(res == MPU6050_IIC_ADDRESS)
	{
			IIC_start();
			IIC_sendByte(MPU6050_Writ_ADDR);
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
			IIC_sendByte(0x6B);//��Դ����Ĵ���1
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
			IIC_sendByte(0x01);//����CLKSEL,PLL X��Ϊ�ο�
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
			IIC_stop();
			Delay_ms(1);
			
			IIC_start();
			IIC_sendByte(MPU6050_Writ_ADDR);//���͵�ַ��д��
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
			IIC_sendByte(0x6C);//��Դ����Ĵ���2 
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
			IIC_sendByte(0x00);//���ٶ��������Ƕ�����
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
			IIC_stop();
			Delay_ms(1);
			
			IIC_start();
			IIC_sendByte(MPU6050_Writ_ADDR);//���͵�ַ��д��
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
			IIC_sendByte(0x19);//����Ƶ�ʷ�Ƶ��
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
			IIC_sendByte(0x14);//���ò�����50Hz
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
			IIC_stop();
	
	}
	
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8 acce_value[6];
u8 gyro_value[6];
u16 MPU_Value_AX;
u16 MPU_Value_AY;
u16 MPU_Value_AZ;
u16 MPU_Value_GX;
u16 MPU_Value_GY;
u16 MPU_Value_GZ;

u8 hour,min,sec,ampm;
accValue_t accValue;
timeStamp_t timeStamp;

u8 MPU_Read_Data(void)
{
	u8 i;
	
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
	IIC_sendByte(0x3B);
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
	IIC_start();
	IIC_sendByte(MPU6050_Read_ADDR);
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
	
	for(i=0;i<5;i++)
	{
		acce_value[i] = IIC_readByte(0);
	}
	acce_value[i] = IIC_readByte(1);
	IIC_stop();
	Delay_us(10);
//////////////////////////////////////////////////////////////////////////////////		
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
	IIC_sendByte(0x43);
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
	IIC_start();
	IIC_sendByte(MPU6050_Read_ADDR);
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
	
	for(i=0;i<5;i++)
	{
		gyro_value[i] = IIC_readByte(0);
	}
	gyro_value[i] = IIC_readByte(1);
	IIC_stop();
	
	
	
	MPU_Value_AX = acce_value[0]<<8|acce_value[1];
	MPU_Value_AY = acce_value[2]<<8|acce_value[3];
	MPU_Value_AZ = acce_value[4]<<8|acce_value[5];
	
	MPU_Value_GX = gyro_value[0]<<8|gyro_value[1];
	MPU_Value_GY = gyro_value[2]<<8|gyro_value[3];
	MPU_Value_GZ = gyro_value[4]<<8|gyro_value[5];
	

	accValue.accX = ((float)(int)MPU_Value_AX/16384) *10;
	accValue.accY = ((float)(int)MPU_Value_AY/16384) *10;
	accValue.accZ = ((float)(int)MPU_Value_AZ/16384) *10;

	RTC_Get_Time(&hour,&min,&sec,&ampm);
	timeStamp.hour   = hour;
	timeStamp.minute = min;
	timeStamp.second = sec;
	onSensorChanged(&accValue,&timeStamp);
//	Display_String(105,0,tbuf);
	return 0;
}

float oriValues[3] = {0};
float gravityNew = 0; 
void onSensorChanged(accValue_t *pAccValue,timeStamp_t *timeStamp_p) {  
  accValue_t *p = pAccValue;
  timeStamp_t *time_p = timeStamp_p;
  oriValues[0] = p->accX;
  oriValues[1] = p->accY;
  oriValues[2] = p->accZ;
  //���������ݽ���ƽ���Ϳ����ŵĴ��� 
  gravityNew = (float) sqrt(oriValues[0] * oriValues[0]  
                            + oriValues[1] * oriValues[1] + oriValues[2] * oriValues[2]);  
  DetectorNewStep(gravityNew,time_p);  
}  



float gravityOld = 0;
long timeOfNow = 0;
long timeOfLastPeak = 0; 
long timeOfThisPeak = 0; 
float ThreadValue = (float) 2.0;
float peakOfWave = 0;  
 static u8 stepTempCount =0;
float valleyOfWave = 0; 
float initialValue = (float) 1.3; 
walk_t walkInfo;

void DetectorNewStep(float values,timeStamp_t *timeStam_p) 
{  
  timeStamp_t *time_p = timeStam_p;
  u8 tbuf[40];
  if (gravityOld == 0) 
  {  
    gravityOld = values;  
  } 
  else 
  {  
    if (DetectorPeak(values, gravityOld))//��⵽����
    {  
      timeOfLastPeak = timeOfThisPeak;//�����ϴβ����ʱ��  
      timeOfNow = ((time_p->hour*60+time_p->minute)*60+time_p->second)*1000+time_p->twentyMsCount*20; //��ȡʱ�� ,��ת��Ϊ����
      //�����⵽�˲��壬���ҷ���ʱ����Լ���ֵ�����������ж�Ϊ1�� 
      if (  (timeOfNow - timeOfLastPeak >= 250 )
          //&& (timeOfNow - timeOfLastPeak <= 2000)
          &&(peakOfWave - valleyOfWave >= ThreadValue)
          )
      {  
        timeOfThisPeak = timeOfNow; //���´˴β���ʱ�� 

        /* 
        * ���½���Ĵ������漰���㷨 
        * һ����֪ͨ���½���֮ǰ���������洦��Ϊ�˴�����Ч�˶��� 
        * 1.������¼10�ſ�ʼ�Ʋ� 
        * 2.�����¼��9���û�ͣס����3�룬��ǰ��ļ�¼ʧЧ���´δ�ͷ��ʼ 
        * 3.������¼��9���û������˶���֮ǰ�����ݲ���Ч 
        * */  
        //    mStepListeners.onStep();  
       	stepTempCount++;
         
//		if ((stepTempCount<= 5 )&&(timeOfNow - timeOfLastPeak >= 3000))
//		{
//			stepTempCount =0;
//		}
		 if((stepTempCount> 0)&&(timeOfNow - timeOfLastPeak <= 3000)) //�޸ļƲ���  һ�μ�����
		{
			//Jahol Fan:����2*stepTempCount���Ǹ��������������Ĵ���ԭ�������
			walkInfo.stepCount += 2*stepTempCount;
			stepTempCount = 0;
			sprintf((char*)tbuf,"StepCount:%02d",walkInfo.stepCount); 
			Display_String(5,4,tbuf);
			//����Ҫ�޸ĳɴ�����ֵ�����������㷨�����ӡ�Ʋ���Ϣ	

		}
		else
		{
			//do nothing
		}


      }  
      //���·�ֵ,����:��ֵ����һֱ��󣬲��ܱ�С?
      if (timeOfNow - timeOfLastPeak >= 250  
          && (peakOfWave - valleyOfWave >= initialValue)) 
      {  
            timeOfThisPeak = timeOfNow;  
            ThreadValue = Peak_Valley_Thread(peakOfWave - valleyOfWave);//���·�ֵ  
      }  
    }  
  }  
  gravityOld = values;  
}  

u8 isDirectionUp = 0;
u8 lastStatus = 0; 
int continueUpCount = 0;
int continueUpFormerCount = 0; 

u8 DetectorPeak(float newValue, float oldValue) 
{  
  lastStatus = isDirectionUp;  
  if (newValue >= oldValue) //�������ݳ���������
  {  
    isDirectionUp = 1;  
    continueUpCount++;  
  }
  else						//���ݳ��½�����
  {  
    continueUpFormerCount = continueUpCount;  
    continueUpCount = 0;  
    isDirectionUp = 0;  
  }  
  
  if ((!isDirectionUp) && lastStatus  
      && (continueUpFormerCount >= 2 || oldValue >= 20))
      //��continueUpFormerCount >= 2 �ĳ�continueUpFormerCount >= 1
     
  {  
    peakOfWave = oldValue;  
    return 1;  
  } 
  else if ((!lastStatus) && isDirectionUp) 
  {  
    valleyOfWave = oldValue;  
    return 0;  
  }
  else
  {  
    return 0;  
  }  
} 


int tempCount = 0; 
float tempValue[4] ={0}; 

float Peak_Valley_Thread(float value) 
{  
  float tempThread = ThreadValue; 
  u8 i = 0;
  if (tempCount < 4)
  {  
    tempValue[tempCount] = value;  
    tempCount++;  
  } 
  else 
  {  
    tempThread = averageValue(tempValue, 4);//���㷧ֵ  
    for ( i = 1;i < 4;i++)//������λ����
    {  
      tempValue[i - 1] = tempValue[i];  
    }  
    tempValue[4 - 1] = value;  
  }  
  return tempThread;  
  
}  



float averageValue(float value[], int n)
{  
  float ave = 0;  
  u8 i =0;
  for ( i = 0; i < n; i++)
  {  
    ave += value[i];//���  
  }  
  ave = ave / 4;//��ƽ��ֵ  
  if (ave >= 8)  
    ave = (float) 4.3;  //????
  else if (ave >= 7 && ave < 8)  
    ave = (float) 3.3;  
  else if (ave >= 4 && ave < 7)  
    ave = (float) 2.3;  
  else if (ave >= 3 && ave < 4)  
    ave = (float) 2.0;  
  else
  {  
    ave = (float) 1.3;  
  }  
  return ave;  
}









