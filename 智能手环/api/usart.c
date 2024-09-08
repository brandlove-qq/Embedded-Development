#include "usart.h"	  
#include "stdio.h"

void Usart_Init(void)
{
	//波特率计算
	float DIV = (84000000)/(16*115200);
	u32 DIV_M = (u32)DIV ;
	u32 DIV_F = ((DIV-DIV_M)*16+0.5f);
	
	RCC->AHB1ENR |= 0x1;
	GPIOA->MODER &=~(0x0F<<(2*9));
	GPIOA->MODER |=(0x0A<<(2*9));   //复用模式
	GPIOA->OTYPER |= 1<<9;//输出开漏
	GPIOA->OSPEEDR &= ~(0x3<<18);
	GPIOA->OSPEEDR |= ~(0x2<<18);
	GPIOA->PUPDR &= ~(0x0f<<(2*9));
	GPIOA->PUPDR |= (0x05<<(2*9));//配置上拉
	
	RCC->APB2ENR |= 0x01<<4;//打开usart时钟
	GPIOA->AFR[1] &=~(0xFF<<(4*(9-8)));
	GPIOA->AFR[1] |=(0x77<<(4*(9-8)));
	
	USART1->CR1 = 0;//整个寄存器清0
	USART1->CR1 |= 0x03<<2;//使能TX RX
	USART1->BRR = (DIV_M << 4) | DIV_F;//波特率配置
	USART1->CR2 = 0;
	USART1->CR1 |= 1<<13;//使能usart
	
	//中断
	NVIC_SetPriority (USART1_IRQn,3);//设置优先级
	NVIC_EnableIRQ (USART1_IRQn);//使能
	USART1->CR1 |= 1<<5;//打开中断开关
}

/************************
发送字符
**************************/
void Usart_SendByte(u8 dat)
{
	while(!(USART1->SR & (1<<7)));
	USART1->DR = dat;
}
/************************
发送字符串
**************************/	
void Usart_Str(u8 *p)
{
	while(*p != '\0')
	{
		Usart_SendByte(*p++);
	}
}
/**************************
格式化输出printf()
***************************/
int fputc(int ch,FILE *F)
{
	while(!(USART1->SR & (1<<7)));
	USART1->DR = ch;
	return 0;
}

/****************************
接受
*****************************/
u8 Usart_Receive(void)
{
	while(!(USART1->SR & (1<<5)));
	
	return USART1->DR;
}


/***********************
中断服务函数
************************/
void USART1_IRQHandler(void)
{

}






