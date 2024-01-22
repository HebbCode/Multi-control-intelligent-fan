#include "BEEP.h"
void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//1、使能GPIO端口
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	//2、配置GPIO端口
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	//配置PF8
	GPIO_Init(GPIOF,&GPIO_InitStruct);
}
void BEEP_On(void)
{
	GPIO_SetBits(GPIOF, GPIO_Pin_8);
}
void BEEP_Off(void)
{
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);
}
