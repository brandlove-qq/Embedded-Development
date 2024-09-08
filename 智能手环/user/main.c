#include "main.h"
u32 sht_run[2]={0,2000};
u32 rtc_run[2]={0,100};
u32 hp_xl_run[2] = {0,1250};
u32 hp_xy_run[2] = {0,1000};
u32 step_run[2] = {0,20};
int main()
{
	u8 key_val = 0;
	u8 hour,min,sec,ampm;
	 u8 year,month,date,week;
	 u8 tbuf[40],str1[20];
	u8 ph[40];
	SysTick_Config(168000);
	NVIC_SetPriorityGrouping(5);
	Usart_Init();
	Led_Init();
	OLED_Init();
	IIC_init();
	Hp_6_init();
	RTC_Init();
	RTC_Set_WakeUp(4,0);
	Display_String(5,0,"Welcome-You!");
	Display_String(5,4,"waiting...5s...");
	Delay_ms (5000);
	HP_6_OpenRate(); //打开心率测量
//	HP_6_OpenBp();   //打开血压测量
	OLED_Clear();
	MPU6050_Init_Check();
	Display_String(5,0,"Heart-Rate");
	while (1)
	{
		
		if(KEY == 1)
		{
			Delay_ms(10);
			if(KEY == 1)
			{
				while(KEY);
				OLED_Clear();
				key_val++;
				if(key_val > 5)
					key_val = 0;
				if(key_val == 0)
				{
					Display_String(5,0,"Heart-Rate");
							HP_6_CloseBp();//关闭血压测量
							HP_6_OpenRate(); //打开心率测量
				}
				else if(key_val == 1)
				{
					Display_String(5,0,"Blood-Pressre");
							HP_6_CloseRate();//关闭心率测量
							HP_6_OpenBp();   //打开血压测量
				}
				else if(key_val == 2)
				{
					HP_6_CloseBp();//关闭血压测量
					Display_String(5,0,"Step-Mode");
				}
				else if(key_val == 3)
				{
					Display_String(5,0,"Tem-And-Hum ");
				}
				else if(key_val == 4)
				{
					Display_String(5,0,"Time-System");
				}
				else
				{
					Display_String(20,2,"Wish-you-good");
							 Display_String(50,4,"Health!");
				}
			}
		}
		
		switch(key_val){
			case 0:
							LED(1);
							if(hp_xl_run[0] > hp_xl_run[1])//测心率
							{
								HP_6_GetRateResult();
								HP_6_GetResultData(ph);	
								sprintf((char*)str1,"result:%d",ph[7]); // //byte 7是心率结果
								Display_String(30,4,str1); 	
								hp_xl_run[0] = 0;
							}
			break;
			case 1:
							LED(0);
							if(hp_xy_run[0] > hp_xy_run[1])//测血压
							{
								HP_6_GetBpResult();                //获取血压测量状态
								HP_6_GetResultData(ph);
								if(0 == ph[7])                   //byte 7是血压测量的状态:0测量中，1测量完成、2测量失败
								Display_String(30,2,"testing...");  
								else if(1 == ph[7])  
								{
									sprintf((char*)str1,"%d/%d", ph[10], ph[11]);//byte 10是血压测量结果的高压,byte 11是血压测量结果的低压	
									Display_String(30,4,str1); 		 
								}		 
								else if(2 == ph[7])                  
								Display_String(30,6,"fail!");
								hp_xy_run[0]=0;
							}							
			break;
			case 2:
							LED(1);
							if(step_run[0]>step_run[1])
							{
								step_run[0] = 0;
								MPU_Read_Data();
							}
			break;
			case 3:
							
							LED(0);
							if(sht_run[0]>sht_run[1])//温湿度
							{
								sht_run[0] = 0;
								SHT20_Tem();
								SHT20_Rh();
							}		
			break;
			case 4:
							LED(1);
							if(rtc_run[0]>rtc_run[1])
							 {
								rtc_run [0]=0;
								RTC_Get_Time(&hour,&min,&sec,&ampm);
								sprintf((char *)tbuf ,"time:%02d:%02d:%02d",hour,min,sec);
								Display_String(5,6,tbuf);
								RTC_Get_Date(&year,&month ,&date,&week);
								sprintf((char *)tbuf ,"date:20%02d-%02d-%02d",year ,month ,date );
								Display_String(5,2,tbuf);
								sprintf((char *)tbuf ,"week:%d",week);
								Display_String(5,4,tbuf);
							 }
			break;
			case 5:
							LED(1);
			break;
			default :break;
		}		
		
		
		
	}
}
