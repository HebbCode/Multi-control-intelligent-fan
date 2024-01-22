#ifndef _DHT11_H
#define _DHT11_H

#define DATA_PIN GPIO_Pin_9
#include "stdio.h"
#include "stm32f4xx.h"
#include "DELAY.h"

u8 DHT11_ReadData(u8 *t1,u8 *t2,u8 *h);

#endif
