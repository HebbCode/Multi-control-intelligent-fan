#include "oled.h"
#include "oledfont.h" 
#include "stdlib.h"
#include "delay.h"
void OLED_IIC_Start()
{
	SDA_OUT();	   //sda线输出
	IIC_SDA_1;		  
	IIC_SCL_1;
	Delay_US_REG(4);
	IIC_SDA_0;//  开始跳变，发出开始信号
	Delay_US_REG(4);
	IIC_SCL_0;//钳住I2C总线，准备发送或接收数据 
}

/**********************************************
//IIC Stop
**********************************************/
void OLED_IIC_Stop()
{
	SDA_OUT();//sda线输出
	IIC_SCL_0;
	IIC_SDA_0;
 	Delay_US_REG(4);
	IIC_SCL_1; 
	IIC_SDA_1;//发送I2C总线结束信号
	Delay_US_REG(4);

}

void OLED_IIC_Wait_Ack()
{
	u8 timeout=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA_1;Delay_US_REG(1);	   
	IIC_SCL_1;Delay_US_REG(1); 
	while(READ_SDA)
	{
		timeout++;
		if(timeout>250)
		{
			OLED_IIC_Stop();
		}
	}
	IIC_SCL_0;//时钟输出0 	    
}
/**********************************************
// IIC Write byte
**********************************************/
void Write_IIC_Byte(unsigned char txd)
{
	u8 t;   
	SDA_OUT(); 	    
    IIC_SCL_0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>>7)IIC_SDA_1;
		else IIC_SDA_0;
        txd<<=1; 	  
		Delay_US_REG(2);  
		IIC_SCL_1;
		Delay_US_REG(2); 
		IIC_SCL_0;	
		Delay_US_REG(2);
    }	 
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
	OLED_IIC_Start();  
	Write_IIC_Byte(0x78);   //发送器件地址0XA0,写数据	 
	OLED_IIC_Wait_Ack();    
	Write_IIC_Byte(0x00);	//发送低地址
	OLED_IIC_Wait_Ack(); 													   
	Write_IIC_Byte(IIC_Command); 	//发送字节							   
	OLED_IIC_Wait_Ack(); 				   
	OLED_IIC_Stop();//产生一个停止条件 
	//Delay_US_REG(500);
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
	OLED_IIC_Start();  
	Write_IIC_Byte(0x78);   //发送器件地址0XA0,写数据	 
	OLED_IIC_Wait_Ack();    
	Write_IIC_Byte(0x40);	//发送低地址
	OLED_IIC_Wait_Ack(); 													   
	Write_IIC_Byte(IIC_Data); 	//发送字节							   
	OLED_IIC_Wait_Ack(); 				   
	OLED_IIC_Stop();//产生一个停止条件 
	//Delay_US_REG(500);
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)Write_IIC_Data(dat);
	else Write_IIC_Command(dat);
}

//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//得到偏移后的值			
	if(x>Max_Column-1){x=0;y=y+2;}
	if(Char_Size ==16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
	}
	else {	
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WR_Byte(F6x8[c][i],OLED_DATA);
	}
}	  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 size,u8 len)
{     
	u8 numx;
  	if(num == 0)OLED_ShowChar(x,y,'0',size);
	else {
		x+=(len-1)*(size/2);
		while(num)
		{
			numx = num % 10;
			OLED_ShowChar(x,y,numx+'0',size);
			x-=size/2;
			num = num / 10;
		}
	}
} 
//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,const char *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j],Char_Size);
		x+=8;
		if(x>120){x=0;y+=2;}
		j++;
	}
}
//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
	{
		OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
		adder+=1;
    }	
	OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
		adder+=1;
    }					
}
void OLED_Show(void){
	OLED_Clear(); 
	OLED_ShowCHinese(18,0,0);//温
	OLED_ShowCHinese(34,0,1);//湿
	OLED_ShowCHinese(50,0,2);//度
	OLED_ShowCHinese(64,0,3);//检
	OLED_ShowCHinese(82,0,4);//测
	OLED_ShowCHinese(98,0,5);//测
	OLED_ShowString(10,2,"T|H:",16);
	OLED_ShowString(10,4,"FAN:",16);
	OLED_ShowString(6,6,"MODE:",16);
//	OLED_ShowChar(47,6,':',16);//显示ASCII字符
}
//初始化SSD1306					    
void OLED_Init(void)
{ 	

 	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

	Delay_MS_REG(200);
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
} 
