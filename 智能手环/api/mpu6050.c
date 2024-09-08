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
	IIC_sendByte(MPU6050_Writ_ADDR);//发送地址（写）
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x6B);//电源管理寄存器1
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x80);//复位MPU6050
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_stop();
	Delay_ms(100);
	
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);//发送地址（写）
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x6B);//电源管理寄存器1
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x00);//唤醒MPU6050 
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_stop();
	Delay_ms(1);
	
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);//发送地址（写）
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x1C);//加速度计配置寄存器
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x18);//陀螺仪传感器,±2000dps
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_stop();
	Delay_ms(1);
	
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);//发送地址（写）
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x1C);//加速度计配置寄存器
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x00);//加速度传感器,±2g
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_stop();
	Delay_ms(1);
	
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);//发送地址（写）
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x38);//中断使能寄存器
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x00);//关闭所有中断
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_stop();
	Delay_ms(1);
	
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);//发送地址（写）
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x19);//采样频率分频器
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x14);//设置采样率50Hz
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
	IIC_sendByte(0x6A);//用户控制寄存器
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x00);//I2C主模式关闭
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
	IIC_sendByte(0x23);//FIFO使能寄存器
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x00);//关闭FIFO
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
	IIC_sendByte(0x37);//中断/旁路设置寄存器
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x80);//INT引脚低电平有效
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_stop();
	Delay_ms(1);
	/*********验证器件ID*******/
	IIC_start();
	IIC_sendByte(MPU6050_Writ_ADDR);
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_sendByte(0x75);////器件ID寄存器
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	IIC_start();
	IIC_sendByte(MPU6050_Read_ADDR);//读命令
	if(IIC_waitAck())
	{
		IIC_stop ();
		return 1;
	}
	res = IIC_readByte(1);//读取数据,发送nACK 
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
			IIC_sendByte(0x6B);//电源管理寄存器1
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
			IIC_sendByte(0x01);//设置CLKSEL,PLL X轴为参考
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
			IIC_stop();
			Delay_ms(1);
			
			IIC_start();
			IIC_sendByte(MPU6050_Writ_ADDR);//发送地址（写）
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
			IIC_sendByte(0x6C);//电源管理寄存器2 
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
			IIC_sendByte(0x00);//加速度与陀螺仪都工作
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
			IIC_stop();
			Delay_ms(1);
			
			IIC_start();
			IIC_sendByte(MPU6050_Writ_ADDR);//发送地址（写）
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
			IIC_sendByte(0x19);//采样频率分频器
			if(IIC_waitAck())
			{
				IIC_stop ();
				return 1;
			}
			IIC_sendByte(0x14);//设置采样率50Hz
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
  //对三轴数据进行平方和开根号的处理 
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
    if (DetectorPeak(values, gravityOld))//检测到波峰
    {  
      timeOfLastPeak = timeOfThisPeak;//更新上次波峰的时间  
      timeOfNow = ((time_p->hour*60+time_p->minute)*60+time_p->second)*1000+time_p->twentyMsCount*20; //获取时间 ,并转化为毫秒
      //如果检测到了波峰，并且符合时间差以及阈值的条件，则判定为1步 
      if (  (timeOfNow - timeOfLastPeak >= 250 )
          //&& (timeOfNow - timeOfLastPeak <= 2000)
          &&(peakOfWave - valleyOfWave >= ThreadValue)
          )
      {  
        timeOfThisPeak = timeOfNow; //更新此次波峰时间 

        /* 
        * 更新界面的处理，不涉及到算法 
        * 一般在通知更新界面之前，增加下面处理，为了处理无效运动： 
        * 1.连续记录10才开始计步 
        * 2.例如记录的9步用户停住超过3秒，则前面的记录失效，下次从头开始 
        * 3.连续记录了9步用户还在运动，之前的数据才有效 
        * */  
        //    mStepListeners.onStep();  
       	stepTempCount++;
         
//		if ((stepTempCount<= 5 )&&(timeOfNow - timeOfLastPeak >= 3000))
//		{
//			stepTempCount =0;
//		}
		 if((stepTempCount> 0)&&(timeOfNow - timeOfLastPeak <= 3000)) //修改计步数  一次计两步
		{
			//Jahol Fan:这里2*stepTempCount，是根据数据现象做的处理，原因不清楚。
			walkInfo.stepCount += 2*stepTempCount;
			stepTempCount = 0;
			sprintf((char*)tbuf,"StepCount:%02d",walkInfo.stepCount); 
			Display_String(5,4,tbuf);
			//这里要修改成带返回值，而不是在算法里面打印计步信息	

		}
		else
		{
			//do nothing
		}


      }  
      //更新阀值,问题:阀值不会一直变大，不能变小?
      if (timeOfNow - timeOfLastPeak >= 250  
          && (peakOfWave - valleyOfWave >= initialValue)) 
      {  
            timeOfThisPeak = timeOfNow;  
            ThreadValue = Peak_Valley_Thread(peakOfWave - valleyOfWave);//更新阀值  
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
  if (newValue >= oldValue) //采样数据呈上升趋势
  {  
    isDirectionUp = 1;  
    continueUpCount++;  
  }
  else						//数据呈下降趋势
  {  
    continueUpFormerCount = continueUpCount;  
    continueUpCount = 0;  
    isDirectionUp = 0;  
  }  
  
  if ((!isDirectionUp) && lastStatus  
      && (continueUpFormerCount >= 2 || oldValue >= 20))
      //把continueUpFormerCount >= 2 改成continueUpFormerCount >= 1
     
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
    tempThread = averageValue(tempValue, 4);//计算阀值  
    for ( i = 1;i < 4;i++)//线性移位更新
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
    ave += value[i];//求和  
  }  
  ave = ave / 4;//求平均值  
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









