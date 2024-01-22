#include "FAN.h"
u16 FAN_State = 0;
void FAN_Init()// DCMI_VSYNC PB7 TIM4OC2
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	// 1、使能GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	// 2、配置GPIO为复用
	GPIO_InitStruct.GPIO_Mode	=	GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin 	=	GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd 	=	GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	// 3、指定复用功能
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10, GPIO_AF_TIM2);
	// 4、使能定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	// 5、配置定时器
	TIM_TimeBaseInitStruct.TIM_ClockDivision	=	TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode 		=	TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period 			=	1000 -1 ; // 重装值
	TIM_TimeBaseInitStruct.TIM_Prescaler		=	84 - 1; // 分频
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=	0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	// 6、配置输出比较单元
	TIM_OCInitStruct.TIM_OCIdleState	=	TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCMode			=	TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNPolarity 	=	TIM_OCNPolarity_Low;
	TIM_OCInitStruct.TIM_OutputNState	=	TIM_OutputNState_Enable;
	TIM_OCInitStruct.TIM_OutputState 	=	TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCNIdleState 	=	TIM_OCNIdleState_Set;
	TIM_OCInitStruct.TIM_Pulse			=	0;	//PWM比较值

	TIM_OC3Init(TIM2, &TIM_OCInitStruct);
	// 7、定时器使能
	TIM_Cmd(TIM2, ENABLE);
	FAN_State = 0;
}
void FAN_Contorl(FANSpeed n){
	TIM_SetCompare3(TIM2, n);
	FAN_State = n;
}

