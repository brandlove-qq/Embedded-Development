#include "oled.h"
#include "font_16x16.h"

volatile uint64_t local_time;
void Oled_Delay(u32 time)
{
	u32 tim = local_time;
	while((local_time-tim)<time);
}


void Oled_Init(void)
{
	RCC->AHB1ENR |= 0x3;//打开pa pb时钟
	
	GPIOA->MODER &= 0x3fffffff;
	GPIOA->MODER |= 0x40000000;
	GPIOA->OTYPER &= 0x7fff;
	GPIOA->OSPEEDR &= 0x3fffffff;
	GPIOA->OSPEEDR |= 0x80000000;//pa15 通用推挽输出
	
	GPIOB->MODER &= ~(0x3<<14);
	GPIOB->MODER |= 0x1<<14;
	GPIOB->OTYPER &= ~(0x1<<7);
	GPIOB->OSPEEDR &= ~(0x3<<14);
	GPIOB->OSPEEDR |= 0x2<<14;//pb7 通用推挽输出
	
	GPIOB->MODER &= 0xf3ffffff;
	GPIOB->MODER |= 0x04000000;
	GPIOB->OTYPER &= 0xdfff;
	GPIOB->OSPEEDR &= 0xf3ffffff;
	GPIOB->OSPEEDR |= 0x08000000;//pb13 通用推挽输出
	
	GPIOB->MODER &= ~(0x33<<6);
	GPIOB->MODER |= 0x22<<6;
	GPIOB->OTYPER &= ~(0x5<<3);
	GPIOB->OSPEEDR &= ~(0x33<<6);
	GPIOB->OSPEEDR |= 0x22<<6;//pb3 pb5 复用推挽输出
	GPIOB->AFR[0] &= ~(0xf0f<<12);
	GPIOB->AFR[0] |= 0x505<<12;
	
	RCC->APB2ENR |= 0x1<<12;
	SPI1->CR1 = 0;
	SPI1->CR1 |= 0x3<<8;//软件从设备
	SPI1->CR1 |= 0x7<<3;//256分频
	SPI1->CR1 |= 1<<2;//主模式
	SPI1->CR1 |= 1<<6;//使能
}


u8 SPI1_Receive_SendByte(u8 dat)
{
	while(!(SPI1->SR&(1<<1)));
	SPI1->DR = dat;
	while(!(SPI1->SR&1));
	return SPI1->DR;
}

void OLED_SendCmd(u8 dat)
{
	
	OLED_CS(0);
	OLED_DC(0);
	SPI1_Receive_SendByte(dat);
	OLED_CS(1);	
}
void OLED_SendData(u8 dat)
{	
	
	OLED_CS(0);
	OLED_DC(1);
	SPI1_Receive_SendByte(dat);
	OLED_CS(1);	
}

void OLED_Init(void)
{
	Oled_Init();
	
	
	OLED_RES(1);
	Oled_Delay(100);
	OLED_RES(0);
	Oled_Delay(100);
	OLED_RES(1);
	Oled_Delay(100);
	OLED_SendCmd(0xAE); /*display off*/
	OLED_SendCmd(0x00); /*set lower column address*/
	OLED_SendCmd(0x10); /*set higher column address*/
	OLED_SendCmd(0x40); /*set display start line*/
	OLED_SendCmd(0xB0); /*set page address*/
	OLED_SendCmd(0x81); /*contract control*/
	OLED_SendCmd(0x66); /*128*/
	OLED_SendCmd(0xA1); /*set segment remap*/
	OLED_SendCmd(0xA6); /*normal / reverse*/
	OLED_SendCmd(0xA8); /*multiplex ratio*/
	OLED_SendCmd(0x3F); /*duty = 1/64*/
	OLED_SendCmd(0xC8); /*Com scan direction*/
	OLED_SendCmd(0xD3); /*set display offset*/
	OLED_SendCmd(0x00);
	OLED_SendCmd(0xD5); /*set osc division*/
	OLED_SendCmd(0x80);
	OLED_SendCmd(0xD9); /*set pre-charge period*/
	OLED_SendCmd(0x1f);
	OLED_SendCmd(0xDA); /*set COM pins*/
	OLED_SendCmd(0x12);
	OLED_SendCmd(0xdb); /*set vcomh*/
	OLED_SendCmd(0x30);
	OLED_SendCmd(0x8d); /*set charge pump enable*/
	OLED_SendCmd(0x14);
	OLED_SendCmd(0xA4);
	OLED_SendCmd(0xA6);
	OLED_SendCmd(0xAF); /*display ON*/
	OLED_Clear();
	
}
void OLED_Set_xy(u8 x,u8 y)
{
	OLED_SendCmd(0xB0+y);
	OLED_SendCmd(0x10|((x>>4)&0xf));
	OLED_SendCmd(x&0xf);

}
void OLED_Clear(void)
{
	u8 j,i;
	for(j=0;j<8;j++)
	{
		OLED_Set_xy(0,j);
		for(i=0;i<132;i++)
		{
			
			OLED_SendData(0x00);
		}
	}
}

void Display_Byte(u8 x,u8 y,const u8 *b)
{
	u8 i,j;
	for(i=0;i<2;i++)
	{
		OLED_Set_xy(x,y+i);
		for(j=0;j<8;j++)
			OLED_SendData(*b++);
	}
}
void Display_CH(u8 x,u8 y,const u8 *ch)
{
	u8 i,j;
	for(i=0;i<2;i++)
	{
		OLED_Set_xy(x,y+i);
		for(j=0;j<16;j++)
			OLED_SendData(*ch++);
	}
}

//void Display_CHLib(u8 x,u8 y,u16 *ch)
//{
//	u8 i,j;
//	u32 addr = HZ16X16ADDR+(((((*ch)>>8)-0xa1)*94+(((*ch)&0xff)-0xa1))*32);
//	for(i=0;i<2;i++)
//	{
//		OLED_Set_xy(x,y+i);
//		for(j=0;j<16;j++)
//			OLED_SendData(Flash_ReadByte(addr++));
//	}
//}

void Display_String(u8 x,u8 y,unsigned char *str)
{
	u32 i;
	while(*str!='\0')
	{
		if(x>124) {x = 0;y += 2;}
		if(y>6) y = 0;
		if(*str<0xa0)
		{
			for(i=0;i<ASCII_NUM();i++)
			{
				if(*str==zf_inder[i])
				{
					Display_Byte(x,y,&ASCII_16X8[i*16]);
					x += 8;
					break;
				}
			}

			str ++;
		}
		else
		{
			i = 0;
			while(CH_inder[i]!='\0')
			{
				if((*str==CH_inder[i])&&((*++str)==CH_inder[i+1]))
				{
					Display_CH(x,y,&CH[(i/2)*32]);
					x += 16;
					break;
				}
				i += 2;
			}	
			str ++;
		}
		
	}
}

void Print_BMP(u8 x,u8 y,u8 wight,u8 hight,const unsigned char *bmp)
{
	u8 i,j;
	for(i=0;i<(hight/8);i++)
	{
		OLED_Set_xy(x,y+i);
		for(j=0;j<wight;j++)
		{
			OLED_SendData(*bmp++);
		}
	}
}


//void Display_StringLib(u8 x,u8 y,const unsigned char *str)
//{
//	u32 i;
//	u16 hz;
//	while(*str!='\0')
//	{
//		if(x>119) {x = 0;y += 2;}
//		if(y>6) y = 0;
//		if(*str<0xa0)
//		{
//			for(i=0;i<ASCII_NUM();i++)
//			{
//				if(*str==zf_inder[i])
//				{
//					Display_Byte(x,y,&ASCII_16X8[i*16]);
//					x += 8;
//					break;
//				}
//			}

//			str ++;
//		}
//		else
//		{		
//				hz = ((*str++)<<8)|(*str); 
//				Display_CHLib(x,y,&hz);
//					x += 16;							
//			str ++;
//		}
//		
//	}
//}






