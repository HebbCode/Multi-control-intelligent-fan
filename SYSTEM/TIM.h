#ifndef TIM_H
#define TIM_H

#include "stm32f4xx.h"
#include "DELAY.h"
void TIM13_Init(uint32_t TIM_Period,uint16_t TIM_Prescaler);
void TIM14_PWMLED0_Init(void);
void LED0_Breath(void);
#endif 

