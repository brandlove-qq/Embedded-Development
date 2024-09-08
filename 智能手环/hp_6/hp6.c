/***********************************************************************************************************************
Copyright 2008 - 2016 深圳市信盈达电子有限公司. All rights reserved.
文件名:        hp6.c
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

#include <stdbool.h>
#include <stdint.h>
#include "hp6.h"
#include "iic.h"
#include "delay.h"
//#include "uart.h"

/****************************************************************
*stm32f4平台的头文件
*/
#include "stm32f4xx.h"
/****************************************************************
*crc校验表
*/
const uint16_t crc16_tab[256] =
{
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,   
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,   
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,   
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,   
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,   
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,   
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,   
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,   
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,   
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,   
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,   
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,   
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,   
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,   
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,   
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,   
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,  
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,   
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,   
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,   
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,   
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,   
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,   
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,   
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,   
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,   
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,   
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,   
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,   
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,   
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,   
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 
};
/****************************************************************
* 宏定义
*/
#define PROTOCLO_HEADER			0xc8d7b6a5
#define PROTOCLO_CMD_INDEX		4
#define PROTOCLO_DATA_INDEX		5
#define PROTOCLO_BUF_MAX_LEN	24

#define HP_6_IIC_ADDRESS 		0x66
#define HP_6_READ_BIT       (0x01)        //!< If this bit is set in the address field, transfer direction is from slave to master.





/****************************************************************
* 全局变量
*/

/****************************************************************
* 本地静态变量
*/
static uint8_t tx_buf[PROTOCLO_BUF_MAX_LEN] = {0};
static uint8_t rx_buf[PROTOCLO_BUF_MAX_LEN] = {0};	

//开启血压测量
const uint8_t cmd_bp_open[]=
{0xc8,0xd7,0xb6,0xa5,0x90,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//关闭血压测量
const uint8_t cmd_bp_close[]=
{0xc8,0xd7,0xb6,0xa5,0x90,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//获取血压测量结果
const uint8_t cmd_bp_result[]=
{0xc8,0xd7,0xb6,0xa5,0x90,0x02,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//开启心率测量
const uint8_t cmd_rate_open[]=
{0xc8,0xd7,0xb6,0xa5,0xD0,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//关闭心率测量
const uint8_t cmd_rate_close[]=
{0xc8,0xd7,0xb6,0xa5,0xD0,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//获取心率测量结果
const uint8_t cmd_rate_result[]=
{0xc8,0xd7,0xb6,0xa5,0xD0,0x02,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//获取ADC 数据
const uint8_t cmd_get_adc[]=
{0xc8,0xd7,0xb6,0xa5,0x91,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//设置低功耗
const uint8_t cmd_set_powersaving[]=
{0xc8,0xd7,0xb6,0xa5,0x70,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//获取版本信息
const uint8_t cmd_get_version[]=
{0xc8,0xd7,0xb6,0xa5,0xa2,0x02,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/****************************************************************
* 本地函数声明
*/
/****************************************************************
** Function name:			Hp_6_PowerUp
** Descriptions:      EN脚控制
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/
void Hp_6_init(void)
{
	RCC->AHB1ENR |= 1<<1;			    //PB端口时钟使能。 第六章
	
	GPIOB->MODER &= ~(3 << 30); 	//32bit,清零
	GPIOB->MODER |= 1 << 30;    	//配置PB15 IO口，为通用输出功能
	
	GPIOB->OTYPER &= ~(1 << 15);	  //配置PB15为推挽输出
	
	GPIOB->OSPEEDR &= ~(3 << 30);
	GPIOB->OSPEEDR |= 2 << 30;  	//配置PB15的输出速度 50MHZ
	

  Hp_6_PowerON();	

}

/****************************************************************
** Function name:			Crc16
** Descriptions:      crc校验 
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:      2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/
uint16_t Crc16(uint8_t *data,uint16_t len)
{
    uint16_t crc16 = 0xFFFF;
    uint32_t uIndex ; //CRC查询表索引
    while (len --)
    {
        uIndex = (crc16&0xff) ^ ((*data) & 0xff) ; //计算CRC
				data = data + 1;
        crc16 = ((crc16>>8) & 0xff) ^ crc16_tab[uIndex];
    }
    return crc16 ;//返回CRC校验值
}
/****************************************************************
** Function name:			Hp_6_I2CWrite
** Descriptions:       
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/
bool Hp_6_I2CWrite(uint8_t Hp_6_address, uint8_t *pData, uint8_t bytes)
{
  uint8_t i;
  IIC_start();
  IIC_sendByte(Hp_6_address<<1);
  IIC_waitAck();
  for(i=0;i<24;i++)
  {
    IIC_sendByte(*(pData+i));
    IIC_waitAck();
  }
  IIC_stop();	
	return 0;
}
/****************************************************************
** Function name:			HP_6_Delay_ms
** Descriptions:            延时函数，单位毫秒
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/
bool Hp_6_I2CRead(uint8_t Hp_6_address, uint8_t *pData, uint8_t bytes)
{
  uint8_t i;
  IIC_start();
  IIC_sendByte((Hp_6_address<<1)|0x01);
  IIC_waitAck();
  for(i=0;i<23;i++)
  {
    /* 从机就会给出数据 */
    *(pData+i) = IIC_readByte(0);      
  }
  /* 从机就会给出数据 */
  *(pData+23) = IIC_readByte(1);//注意：读取byte[23]数据无需应答    
  IIC_stop();
	return 0;
}
/****************************************************************
** Function name:			HP_6_Delay_ms
** Descriptions:            延时函数，单位毫秒
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
void HP_6_Delay_ms(uint16_t ms)
{
  uint16_t time_delay = ms;
  Delay_ms(time_delay); //延时函数，单位毫秒
}
/****************************************************************
** Function name:			HP_6_GetResultData
** Descriptions:            	获取结果数据
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint16_t HP_6_GetResultData(uint8_t *data)
{
  uint8_t i = 0;
  
  if(!data)
  {
    return 0;
  }
  
  for(i=0; i < PROTOCLO_BUF_MAX_LEN; i++)
  {
    data[i] = rx_buf[i];
  }
  
  return PROTOCLO_BUF_MAX_LEN;
}
/****************************************************************
** Function name:			HP_6_SendCmd
** Descriptions:            	发送命令到从机并获取结果
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_SendCmd(uint8_t *tx_buf, uint8_t *rx_buf)
{
  uint8_t state = HP_6_SUCCESS;
  uint16_t crc;	
  
  crc = Crc16(&tx_buf[4], 18);	//小端序
  *(uint16_t*)(&tx_buf[22]) = crc;
  
  Hp_6_I2CWrite(HP_6_IIC_ADDRESS, tx_buf, 24);
  Delay_ms(5);
  Hp_6_I2CRead(HP_6_IIC_ADDRESS, rx_buf, 24);
  
  crc = *(uint16_t*)(&rx_buf[22]);	//小端序
  if(crc != Crc16(&rx_buf[4], 18))	//校验失败
  {
    state = HP_6_FAILURE;
  }

  return state;
}

/****************************************************************
** Function name:			HP_6_GetRateResult
** Descriptions:            	获取心率测量结果
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_GetRateResult(void)
{
  uint32_t i;	
  for(i=0; i < PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_rate_result[i];		
  }
  
  return HP_6_SendCmd( tx_buf, rx_buf);   
}

/****************************************************************
** Function name:			HP_6_OpenRate
** Descriptions:            	开启心率测量
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_OpenRate(void)
{
  uint32_t i;
  
  for( i = 0; i < PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_rate_open[i];
  }
  
  return HP_6_SendCmd( tx_buf, rx_buf);       
}

/****************************************************************
** Function name:			HP_6_OpenRate
** Descriptions:            	关闭心率测量
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_CloseRate(void)
{
  uint32_t i;
  
  for( i = 0; i < PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_rate_close[i];
  }
  
  return HP_6_SendCmd( tx_buf, rx_buf);       
}

/****************************************************************
** Function name:			HP_6_GetBpResult
** Descriptions:            	获取血压测量结果
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_GetBpResult(void)
{
  uint32_t i;	
  
  for(i=0; i < PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_bp_result[i];	
  }
  
  return HP_6_SendCmd( tx_buf, rx_buf);  
}

/****************************************************************
** Function name:			HP_6_OpenBp
** Descriptions:            	开启心率测量
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_OpenBp(void)
{
  uint32_t i;
  
  for( i = 0; i < PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_bp_open[i];
  }
  
  return HP_6_SendCmd( tx_buf, rx_buf);       
}

/****************************************************************
** Function name:			HP_6_CloseBp
** Descriptions:            	关闭血压测量
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_CloseBp(void)
{
  uint32_t i;
  
  for( i = 0; i < PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_bp_close[i];
  }
  
  return HP_6_SendCmd( tx_buf, rx_buf);       
}

/****************************************************************
** Function name:			HP_6_GetADC
** Descriptions:            	关闭血压测量
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_GetADC(uint8_t num, uint8_t id)
{
  uint32_t i;
  uint16_t crc;
  
  for( i = 0; i < PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_get_adc[i];
  }
  tx_buf[PROTOCLO_DATA_INDEX] =  num;
  tx_buf[PROTOCLO_DATA_INDEX+1] =  id;
  
  crc = Crc16(&tx_buf[4],18);	
  *(uint16_t*)(&tx_buf[22]) = crc;
  
  return HP_6_SendCmd( tx_buf, rx_buf);      
}

/****************************************************************
** Function name:			HP_6_PowerSaving
** Descriptions:            设置低功耗
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/ 
uint8_t HP_6_PowerSaving(void)
{
  uint32_t i;
  
  for(i=0; i<PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_set_powersaving[i];
  }
  
  return HP_6_SendCmd( tx_buf, rx_buf);
}

/****************************************************************
** Function name:			HP_6_VersionInfo
** Descriptions:            获取版本信息
** input parameters:
** output parameters:
** Returned value:
** Created by:				Jahol Fan              
** Created Date:           	2016-09-02
**-----------------------------------------------------------------------
** Modified by:
** Modified date:
**-----------------------------------------------------------------------
*****************************************************************/  
uint8_t HP_6_VersionInfo(void)
{
  uint32_t i;
  
  for(i=0; i<PROTOCLO_BUF_MAX_LEN; i++)
  {
    tx_buf[i] = cmd_get_version[i];
  }
  
  return HP_6_SendCmd( tx_buf, rx_buf);	    
}

