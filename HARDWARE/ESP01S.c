#include "ESP01S.h"
extern Queue RevBuf;
extern uint8_t DisplayResponse;
extern u8 ProgramStatus; // 程序状态
u8 ATRevTest(void)
{
	int i;
	Queue *Q = &RevBuf;
	for(i=Q->front; i!=Q->rear;)
	{
		if(Q->base[i] == 'O')
		{
			i = (i+1)%MAXSIZE;
			if(Q->base[i] == 'K')
			{
				ClearQueue(Q); 
				return 1;
			}
			i--;
		}
		i = (i+1)%MAXSIZE;
	}
	ClearQueue(Q);
	return 0;
}

void ESP01S_Init(void)
{
	u8 ErrorNum = 0;
	char WiFi[128] = "";
	// 1 测试模块是否正常
	USART_SendDatas(USART2,(u8 *)AT_TEST,strlen(AT_TEST));
	DelayMs(WAIT_TIME);
	if(ATRevTest())printf("0 AT Response Successful\r\n");
	else {ErrorNum = 1;goto L;}
	
	//2 设置工作模式为单主机模式
	DelayMs(WAIT_TIME);
	USART_SendDatas(USART2,(u8*)AT_SetMode1,strlen(AT_SetMode1));
	DelayMs(WAIT_TIME);
	if(ATRevTest())printf("1 Set AT_Mode STA\r\n");
	else {ErrorNum = 2;goto L;}
	
	//3 连接WiFi
	sprintf(WiFi, "AT+CWJAP=\"%s\",\"%s\"\r\n",WiFi_Name,WiFi_PW);
	DelayMs(WAIT_TIME * 3);
	USART_SendDatas(USART2,(u8*)WiFi,strlen(WiFi));
	DelayMs(WAIT_TIME * 10);
	if(ATRevTest())printf("2 Connetc WiFi(%s) Success\r\n",WiFi_Name);
	else {ErrorNum = 3;goto L;}
	
	//4 连接TCP服务器
	sprintf(WiFi, "AT+CIPSTART=\"%s\",\"%s\",%s\r\n",Protocol,TCP_ServerIP,TCP_ServerPort); 
	DelayMs(WAIT_TIME);
	USART_SendDatas(USART2,(u8*)WiFi,strlen(WiFi));
	DelayMs(WAIT_TIME * 3);
	if(ATRevTest())printf("3 Connetc TCP_Server(%s:%s) Success\r\n",TCP_ServerIP,TCP_ServerPort);
	else {ErrorNum = 4;goto L;}
	
	// 5 进入透传模式
	DelayMs(WAIT_TIME);
	USART_SendDatas(USART2,(u8*)AT_EnterPassthroughMode,strlen(AT_EnterPassthroughMode));
	DelayMs(WAIT_TIME);
	if(ATRevTest())printf("4 Enter Passthrough Mode Success\r\n");
	else {ErrorNum = 5;goto L;}
	
	//6 开启透传发送
	DelayMs(WAIT_TIME);
	USART_SendDatas(USART2,(u8*)AT_OpenSendData,strlen(AT_OpenSendData));
	DelayMs(WAIT_TIME);
	if(ATRevTest())printf("5 OpenSendData Success\r\n");
	else {ErrorNum = 6;goto L;}
	
	DelayMs(WAIT_TIME/2);
	USART_SendDatas(USART2,(u8 *)"Device is online",strlen("device is online"));
	
	L:if(ErrorNum)
	{
		ProgramStatus = 0;
		if(ErrorNum == 5)
		{
			USART_SendDatas(USART2,(u8 *)"+++",3);// 退出透传模式
		}
		printf("ERROR:%d",ErrorNum);
		return;
	}
	DisplayResponse = 1;
	ClearQueue(&RevBuf);
	
}
