#include "stm32f4xx.h" // Device header
#include <stdio.h>
#include "DELAY.h"
#include "USART.h"
#include "DHT11.h"
#include "ESP01S.h"
#include "FAN.h"
#include "RADAR.h"
#include "OLED.h"
u8 ProgramStatus = 1; // 程序状态
u8 HumidityControlMode = 1; // 温湿度控制模式
u8 NetControlMode = 0; // 网络控制模式

extern u16 FAN_State; // 风速
extern u8 RADAR_X; // 是否有人在

void All_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	DelayInit();// 延时函数初始化
	DelayMs(1000);
	USART1_Init(115200); // 串口1 初始化
	USART2_Init(115200); // 串口2 初始化
	FAN_Init(); // 风扇初始化
	RADAR_Init(); // 雷达初始化
	OLED_Init();
	DelayMs(500);
	OLED_Show();
	DelayMs(1000);
	ESP01S_Init();// ESP01s初始化
}

int main()
{
	char info[128] = "";
	u8 T1,T2,H;
	All_Init();
	while(ProgramStatus)
	{		
		if(!RADAR_X)// 由雷达传感器判断是否有人
		{
			sprintf(info,"Nobody\r\n");
			OLED_ShowString(50,6,"Nobody",16);
			continue;
		}
		T1 = T2 = H = 0;
		if(DHT11_ReadData(&T1,&T2,&H))printf("DHT11 Read Error\r\n");
		else 
		{
			sprintf(info,"Temperature:%d.%d - Humidity:%d\r\n",T1,T2,H);//获取到正确的信息
			OLED_ShowNum(50,2,T1,16,2);
			OLED_ShowChar(66,2,'.',16);
			OLED_ShowNum(72,2,T2,16,1);
			OLED_ShowChar(86,2,'&',16);
			OLED_ShowNum(100,2,H,16,2);
			if(HumidityControlMode)//湿度控制
			{
				if(H<=50)FAN_Contorl(STOP);
				else if(H<=60)FAN_Contorl(LOW);
				else if(H<=80)FAN_Contorl(MID);
				else if(H<=100)FAN_Contorl(HIGH);
				strcat(info,"DHT11ControlMode\r\n");
				OLED_ShowString(50,6,"DHT11",16);
			}
			else
			{
				strcat(info,"NetControlMode\r\n");
				OLED_ShowString(50,6,"NET",16);
			}// 网络控制
			switch(FAN_State)// 风扇的状态信息
			{
				case LOW:{strcat(info,"FAN_Speed: LOW\r\n");OLED_ShowString(50,4,"LOW",16);break;}
				case MID:{strcat(info,"FAN_Speed: MID\r\n");OLED_ShowString(50,4,"MID",16);break;}
				case HIGH:{strcat(info,"FAN_Speed: HIGH\r\n");OLED_ShowString(50,4,"HIGH",16);break;}
				case STOP:{strcat(info,"FAN_Speed: STOP\r\n");OLED_ShowString(50,4,"STOP",16);break;}
			}
			USART_SendDatas(USART2,(uint8_t *)info,strlen(info));// 将整合的信息通过网络发出
			printf("%s",info); // 通过串口打印
			memset(info,0,128);// 清理
		}
		DelayMs(3000);
	}
	
}

