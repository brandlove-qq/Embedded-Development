#include "usart.h"	  
#include "stdio.h"

void Usart_Init(void)
{
	//�����ʼ���
	float DIV = (84000000)/(16*115200);
	u32 DIV_M = (u32)DIV ;
	u32 DIV_F = ((DIV-DIV_M)*16+0.5f);
	
	RCC->AHB1ENR |= 0x1;
	GPIOA->MODER &=~(0x0F<<(2*9));
	GPIOA->MODER |=(0x0A<<(2*9));   //����ģʽ
	GPIOA->OTYPER |= 1<<9;//�����©
	GPIOA->OSPEEDR &= ~(0x3<<18);
	GPIOA->OSPEEDR |= ~(0x2<<18);
	GPIOA->PUPDR &= ~(0x0f<<(2*9));
	GPIOA->PUPDR |= (0x05<<(2*9));//��������
	
	RCC->APB2ENR |= 0x01<<4;//��usartʱ��
	GPIOA->AFR[1] &=~(0xFF<<(4*(9-8)));
	GPIOA->AFR[1] |=(0x77<<(4*(9-8)));
	
	USART1->CR1 = 0;//�����Ĵ�����0
	USART1->CR1 |= 0x03<<2;//ʹ��TX RX
	USART1->BRR = (DIV_M << 4) | DIV_F;//����������
	USART1->CR2 = 0;
	USART1->CR1 |= 1<<13;//ʹ��usart
	
	//�ж�
	NVIC_SetPriority (USART1_IRQn,3);//�������ȼ�
	NVIC_EnableIRQ (USART1_IRQn);//ʹ��
	USART1->CR1 |= 1<<5;//���жϿ���
}

/************************
�����ַ�
**************************/
void Usart_SendByte(u8 dat)
{
	while(!(USART1->SR & (1<<7)));
	USART1->DR = dat;
}
/************************
�����ַ���
**************************/	
void Usart_Str(u8 *p)
{
	while(*p != '\0')
	{
		Usart_SendByte(*p++);
	}
}
/**************************
��ʽ�����printf()
***************************/
int fputc(int ch,FILE *F)
{
	while(!(USART1->SR & (1<<7)));
	USART1->DR = ch;
	return 0;
}

/****************************
����
*****************************/
u8 Usart_Receive(void)
{
	while(!(USART1->SR & (1<<5)));
	
	return USART1->DR;
}


/***********************
�жϷ�����
************************/
void USART1_IRQHandler(void)
{

}






