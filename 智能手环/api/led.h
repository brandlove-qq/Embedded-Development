#ifndef _LED_H_
#define _LED_H_

#include "stm32f4xx.h"
void Led_Init(void);
#define LED(x) x?(GPIOA->ODR |= 1<<7):(GPIOA->ODR &= ~(1<<7));
#define KEY (GPIOA->IDR>>0 & 1)
#define MOTOR(x) x?(GPIOB->ODR |= 1<<10):(GPIOB->ODR &= ~(1<<10));

#endif
