#ifndef _KEY_H
#define _KEY_H
#include "stm32f4xx.h"
#include "LED.h"
#include "BEEP.h"

void KEY_Init(void);
void KEY_Monitor(void);

void KEY_Monitor_EXTI(void);
void EXTI0_IRQHandler(void);

#endif
