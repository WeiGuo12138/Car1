#ifndef _TIM_H
#define _TIM_H
#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//��ʱ����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/5/25
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
extern TIM_HandleTypeDef TIM1_Handler;      //��ʱ����� 

void TIM1_Init(uint16_t arr,uint16_t psc);
void TIM1_PWM_Init(uint16_t arr,uint16_t psc);
void TIM_SetTIM1Compare2(uint16_t compare);
#endif

