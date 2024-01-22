#include "DHT11.h"

void dht11_GPIO(u8 state)// 板载DHT11 PG9
{	
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	if(state){
		GPIO_InitStructure.GPIO_Pin = DATA_PIN;		
		GPIO_InitStructure.GPIO_Mode= GPIO_Mode_OUT;	
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOG,&GPIO_InitStructure);
		GPIO_SetBits(GPIOG, DATA_PIN);
	}
	else {
		GPIO_InitStructure.GPIO_Pin = DATA_PIN;		
		GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOG,&GPIO_InitStructure);
	}
}
u8 dht11_Check()
{	
	u8 temp;
	// 唤醒信号
	dht11_GPIO(1);
	Delay_MS_REG(5);
	GPIO_ResetBits(GPIOG, DATA_PIN);
	Delay_MS_REG(20);
	GPIO_SetBits(GPIOG, DATA_PIN);
	Delay_US_REG(30);

	dht11_GPIO(0);
	if(GPIO_ReadInputDataBit(GPIOG, DATA_PIN) != 0)return 0;

	temp = 200;
	while(--temp && !GPIO_ReadInputDataBit(GPIOG, DATA_PIN))Delay_US_REG(1);
	if(temp == 0)return 0;
	
	temp = 200;
	while(--temp && GPIO_ReadInputDataBit(GPIOG, DATA_PIN))Delay_US_REG(1);
	if(temp == 0)return 0;

	return 1;
}
u8 dht11_ReadBytes()
{	
	u8 i;
	u8 byt = 0;
    for (i=0; i<8; i++)
    {
        while (GPIO_ReadInputDataBit(GPIOG, DATA_PIN));  //等待低电平，数据位前都有50us低电平时隙
        while (!GPIO_ReadInputDataBit(GPIOG, DATA_PIN)); //等待高电平，开始传输数据位
        Delay_US_REG(40);
        byt <<= 1;         //因高位在前，所以左移byt，最低位补0
        if (GPIO_ReadInputDataBit(GPIOG, DATA_PIN))      //将总线电平值读取到byt最低位中
        {
            byt |= 0x01;
        }
    }
    return byt;
}
u8 DHT11_ReadData(u8 *t1,u8 *t2,u8 *h)
{
	u8 i;
	u8 buf[5];
	if(!dht11_Check()){printf("DHT11 ACK Fail\r\n");return 1;}
	for(i = 0; i < 5; i++)
	{
		buf[i] = dht11_ReadBytes();
	}
	if(buf[0]+buf[1]+buf[2]+buf[3] != buf[4]){printf("DHT11 CheckSum Fail\r\n");return 2;}
	*h = buf[0];
	*t1 = buf[2];
	*t2 = buf[3] ;
	return 0;
}
