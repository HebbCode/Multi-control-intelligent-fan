#include "HC-SR501.h"
void HC_SR501_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	//1、初始化GPIO端口	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
//	
//	//2、设置SYSCFG选择器，使能与选择
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);

//	//3、配置外部中断控制器
//	EXTI_InitStruct.EXTI_Line = EXTI_Line10;
//	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
//	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
//	EXTI_Init(&EXTI_InitStruct);

//	//4、配置NVIC中断控制器，不需要使能，自有时钟
//	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
//	NVIC_Init(&NVIC_InitStruct);
}
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line10) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10))GPIO_SetBits(GPIOF,GPIO_Pin_9);
		else GPIO_ResetBits(GPIOF,GPIO_Pin_9);
		EXTI_ClearFlag(EXTI_Line10);
	}
}

