#include "tim.h"
void TIM13_Init(uint32_t TIM_Period,uint16_t TIM_Prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	//1.使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);
	//2.初始化定时器时基单元
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = TIM_Period;
	TIM_TimeBaseInitStruct.TIM_Prescaler = TIM_Prescaler;
	TIM_TimeBaseInit(TIM13, &TIM_TimeBaseInitStruct);
	//3.中断配置
	TIM_ITConfig(TIM13, TIM_IT_Update, ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
	//4.定时器使能
	TIM_ARRPreloadConfig(TIM13,ENABLE);
	TIM_Cmd(TIM13,ENABLE);
}

/*中断函数*/
void TIM8_UP_TIM13_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM13, TIM_IT_Update)==SET)
	{
		GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
		TIM_ClearITPendingBit(TIM13, TIM_IT_Update);
	}
}

void TIM14_PWMLED0_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	// 1、使能GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	// 2、配置GPIO为复用
	GPIO_InitStruct.GPIO_Mode	=	GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType 	= 	GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin 	=	GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd 	=	GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	// 3、指定复用功能
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9, GPIO_AF_TIM14);
	// 4、使能定时器时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	// 5、配置定时器
	TIM_TimeBaseInitStruct.TIM_ClockDivision	=	TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode 		=	TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period 			=	1000 -1 ; // 重装值
	TIM_TimeBaseInitStruct.TIM_Prescaler		=	84 - 1; // 分频
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=	0;
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseInitStruct);
	// 6、配置输出比较单元
	TIM_OCInitStruct.TIM_OCIdleState	=	TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCMode			=	TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNPolarity 	=	TIM_OCNPolarity_Low;
	TIM_OCInitStruct.TIM_OutputNState	=	TIM_OutputNState_Enable;
	TIM_OCInitStruct.TIM_OutputState 	=	TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCNIdleState 	=	TIM_OCNIdleState_Set;
	TIM_OCInitStruct.TIM_Pulse			=	0;	//PWM比较值

	TIM_OC1Init(TIM14, &TIM_OCInitStruct);
	// 7、定时器使能
	TIM_Cmd(TIM14, ENABLE);
}
void LED0_Breath(void)
{
	int n = 1000,flag = 1;
	SysTick_Init_REG();
	while(1)
	{	
		TIM_SetCompare1(TIM14, n);
		if(flag)n--;
		else n++;
		if(n == 0)flag = 0;
		if(n == 1000) flag = 1;
		Delay_MS_REG(1);
	}
}
