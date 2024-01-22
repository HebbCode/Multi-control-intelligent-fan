#include "DELAY.h"
u32 rdelay_time = 0;
void delayx(UL s)
{
	UL i,temp;
	for(i = 0; i < s ;i++)
	{
		temp = 10000;
		while(temp--);
	}
}
void SysTick_Init_REG(void){
	if(SysTick_Config(SystemCoreClock/1000000) == 1)//1微秒
    {
        while(1);
    }
}
void Delay_MS_REG(u32 n)
{
	Delay_US_REG(n*1000);
}
void Delay_US_REG(u32 n)
{
	rdelay_time = 0;
    while(rdelay_time != n);
}

