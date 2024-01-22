#ifndef __OLED_H
#define __OLED_H			  	 
#include "stm32f4xx.h"
//#include "simulate_iic.h"

// PB8 PB9 
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	  

//-----------------OLED IIC端口定义----------------  					   
#define SDA_IN()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9输出模式

#define IIC_SCL_1  GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define IIC_SCL_0  GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define IIC_SDA_1  GPIO_SetBits(GPIOB, GPIO_Pin_9)
#define IIC_SDA_0  GPIO_ResetBits(GPIOB, GPIO_Pin_9)
#define READ_SDA   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) //输入SDA 

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_8) //SDA IIC接口的时钟信号
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB, GPIO_Pin_8)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_9)//SCL IIC接口的数据信号
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_9)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 size,u8 len);
void OLED_ShowString(u8 x,u8 y, const char  *p,u8 Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void OLED_Show(void);
#endif  
	 



