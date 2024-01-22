#include "USART.h"

Queue RevBuf;// 通过ESP01S接收到的信息 缓冲队列
u8 DisplayResponse = 0;// 是否回显ESP01S串口接收到的信息，同时也决定是否处理信息

extern u8 HumidityControlMode;// 湿度控制模式
extern u8 NetControlMode;// 网络控制模式
extern u8 ProgramStatus;// 程序是否继续运行

//重定向printf到串口1
struct __FILE { int handle; };
FILE  __stdout;
int fputc(int c,FILE* stream)
{
    USART_SendData(USART1,c&0xFF);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)!=SET);
    return 0;
}

void CheckCMD(char *TempBuf)
{
	if(strstr(TempBuf,"C"))
	{
		if(HumidityControlMode)FAN_Contorl(STOP);// 如果当前是湿度控制模式，则需要停止风扇
		HumidityControlMode = !HumidityControlMode;// 状态取反
		NetControlMode = !NetControlMode;// 状态取反
	}
	else if(strstr(TempBuf,"Q"))// 退出程序指令 
	{	
		FAN_Contorl(STOP);// 先将风扇停止
		ProgramStatus = 0;// 将程序状态标记为退出
		USART_SendDatas(USART2,(u8 *)"+++",3);// 退出透传模式
	}
	//else USART_SendDatas(USART2,(u8 *)"ERROR CMD\r\n",11);// 指令错误
}
//串口1 中断函数
void USART1_IRQHandler(void)
{
	uint16_t i;
	char USART1_Recbuf[1024];
	uint16_t USART1_Pointer = 0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)//由来数据引发中断
	{	
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
		{
			// 将串口1的所有数据通过串口2发送出去，这里先暂存缓冲区
			USART1_Recbuf[USART1_Pointer++] = USART_ReceiveData(USART1); 
		}
		for(i = 0; i < USART1_Pointer;i++)
		{
			USART_SendData(USART2,USART1_Recbuf[i]);// 将缓冲区数据发送
			while(USART_GetFlagStatus(USART2 , USART_FLAG_TXE) == RESET);// 等待发送完毕
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);//清理标志位
	}
}
void USART2_IRQHandler(void)
{	
	uint16_t i;
	char USART2_Recbuf[1024],temp;
	uint16_t USART2_Pointer = 0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)//由来数据引发中断
	{	
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
		{
			temp = USART_ReceiveData(USART2); // 将ESP01S(串口2)收到的数据暂存
			if(DisplayResponse)USART2_Recbuf[USART2_Pointer++] = temp; // 如果开启了回显，就放到缓冲区
			else EnQueue(&RevBuf,temp);// 否则放到缓冲队列中去
		}
		//在开启回显后，也就是ESP01S进入到透传模式后，串口就要处理来自服务器发过来的数据了
		if(DisplayResponse)
		{
			USART2_Recbuf[USART2_Pointer++] = '\0';//因为要使用字符串处理函数，这里先
			CheckCMD(USART2_Recbuf); // 是否是切换命令
		}
		if(NetControlMode)//如果进入到了网络模式，就需要对发过来的数据进行解析
		{
			switch(USART2_Recbuf[0] - '0')
			{
				case 0:{FAN_Contorl(STOP);break;}
				case 1:{FAN_Contorl(LOW);break;}
				case 2:{FAN_Contorl(MID);break;}
				case 3:{FAN_Contorl(HIGH);break;}
			}
		}
		// 将ESP01S收到的数据返回在串口1上
		for(i = 0; i < USART2_Pointer;i++)
		{
			USART_SendData(USART1,USART2_Recbuf[i]);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		}
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}
void USART1_Init(u32 BaudRate)// USART1 PA9 | PA10
{
	// 1.GPIO口
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	// 复用
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10, GPIO_AF_USART1);
	// 2.串口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitStruct.USART_BaudRate = BaudRate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);	
	// 3.中断配置
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_SetPriority(USART1_IRQn,1);
	NVIC_Init(&NVIC_InitStruct);
    USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);//使能接收中断
	USART_Cmd(USART1,ENABLE);
}
void USART2_Init(u32 BaudRate) // USART2 PA2 | PA3
{
	// 1.GPIO口
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	// 复用
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2);
	// 2.串口
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_InitStruct.USART_BaudRate = BaudRate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStruct);
	
	// 3.中断配置
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_SetPriority(USART2_IRQn,1);
	NVIC_Init(&NVIC_InitStruct);
    USART_ITConfig(USART2, USART_IT_RXNE,ENABLE);//使能接收中断
	USART_Cmd(USART2,ENABLE);
	InitQueue(&RevBuf);
	
}

void USART_SendDatas(USART_TypeDef * USARTx,uint8_t* SendBuf,uint16_t n)
{
	int i;
	for(i=0;i<n;i++)
	{
		USART_SendData(USARTx,SendBuf[i]);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);//等待发送完成(发送数据寄存器为空 以免覆盖)
	}	
}

