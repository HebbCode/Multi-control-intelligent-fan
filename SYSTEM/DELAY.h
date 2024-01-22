#ifndef _DELAY_H
#define _DELAY_H
#include "main.h"
#include "stm32f4xx.h"
//systick定时器中控制及状态寄存器
#define rSysTickCTL    *((volatile unsigned long*)0xE000E010)
//systick定时器中重装载数值寄存器
#define rSysTickRELOAD *((volatile unsigned long*)0xE000E014)
//ystick定时器中当前数值寄存器
#define rSysTickVAL    *((volatile unsigned long*)0xE000E018)

#define DelayMs Delay_MS_REG
#define DelayUs Delay_US_REG
#define DelayInit SysTick_Init_REG

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

void delayx(UL s);
void SysTick_Init_REG(void);
void Delay_MS_REG(u32 n);
void Delay_US_REG(u32 n);

#endif
