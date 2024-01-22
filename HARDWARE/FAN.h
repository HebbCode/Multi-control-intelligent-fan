#ifndef _FAN_H
#define _FAN_H
#include "stm32f4xx.h"
typedef enum ns{STOP=0,LOW=300,MID=600,HIGH=1000}FANSpeed;

void FAN_Init(void);
void FAN_Contorl(FANSpeed n);

#endif
