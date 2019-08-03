#ifndef _USART_H
#define _USART_H
#include "stm32f1xx_hal.h"

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

void MX_USART3_UART_Init(void);
void MX_USART2_UART_Init(void);

#endif



