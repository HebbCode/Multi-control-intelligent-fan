#include "KEY.h"
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//1、使能GPIO端口
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE,ENABLE);
	//2、初始化GPIO端口
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void KEY_Monitor(void)
{
	u8 State0,State1,State2,State3;
	KEY_Init();
	while(1)
	{
		State0 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
		if(State0 == 0)GPIO_ResetBits(GPIOF, GPIO_Pin_9);
		
		State1 = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2);
		if(State1 == 0)GPIO_ResetBits(GPIOF, GPIO_Pin_10);
		
		State2 = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3);
		if(State2 == 0)GPIO_ResetBits(GPIOE, GPIO_Pin_13);

		State3 = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4);
		if(State3 == 0)GPIO_ResetBits(GPIOE, GPIO_Pin_14);

		if(State0+State1+State2+State3 <=2 )BEEP_On();
		else BEEP_Off();

		LED_CloseAll();
	}
}
void KEY_Monitor_EXTI(void)
{	
	NVIC_InitTypeDef NVIC_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	
	//1、初始化GPIO端口
	KEY_Init();
	
	//2、设置SYSCFG选择器，使能与选择
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	//3、配置外部中断控制器
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);

	//4、配置NVIC中断控制器，不需要使能，自有时钟
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
}
extern u8 FAN_State;
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		if(!FAN_State)
		EXTI_ClearFlag(EXTI_Line0);
	}
}
