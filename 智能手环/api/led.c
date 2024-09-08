#include "led.h"

void Led_Init(void)
{
	RCC->AHB1ENR |= 1<<0;			    	
	GPIOA->MODER &= ~(3 << 14); 	
	GPIOA->MODER |= 1 << 14;    	
	GPIOA->OTYPER &= ~(1 << 7);	 	
	GPIOA->OSPEEDR &= ~(3 << 14);
	GPIOA->OSPEEDR |= 2 << 14;  //led
	LED(1);
	
	GPIOA->MODER &= ~1;
	GPIOA->PUPDR &= ~1;//°´¼ü
	
	RCC->AHB1ENR |= 1<<1;			    	
	GPIOB->MODER &= ~(3 << 20); 	
	GPIOB->MODER |= 1 << 20;    	
	GPIOB->OTYPER &= ~(1 << 10);	 	
	GPIOB->OSPEEDR &= ~(3 << 20);//µç»ú
	GPIOB->OSPEEDR |= 2 << 20;
	
}
