#ifndef _USART_H
#define _USART_H
#include <stdio.h>
#include "stm32f4xx.h" // Device header
#include <string.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "QUEUE.h"
#include "FAN.h"

void USART1_Init(uint32_t BaudRate);
void USART2_Init(uint32_t BaudRate);
void USART_SendDatas(USART_TypeDef * USARTx,uint8_t * SendBuf,uint16_t n);

#endif
