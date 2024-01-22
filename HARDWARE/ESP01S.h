#ifndef _ESP01S_H
#define _ESP01S_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "USART.h"
#include "DELAY.h"
#include "QUEUE.h"



#define WiFi_Name "AP"
#define WiFi_PW "87654321"

#define Protocol "TCP"
#define TCP_ServerIP "192.168.29.185"
#define TCP_ServerPort "6666"

#define WAIT_TIME 500

#define AT_TEST "AT\r\n"
#define AT_SetMode1 "AT+CWMODE=1\r\n"
#define AT_EnterPassthroughMode "AT+CIPMODE=1\r\n"
#define AT_OpenSendData "AT+CIPSEND\r\n"
#define AT_CloseSendData "+++\r\n"

void ESP01S_Init(void);
#endif
