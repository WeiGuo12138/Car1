#ifndef _TIM_H
#define _TIM_H
#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//定时器驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/5/25
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
extern TIM_HandleTypeDef TIM1_Handler;      //定时器句柄 

void TIM1_Init(uint16_t arr,uint16_t psc);
void TIM1_PWM_Init(uint16_t arr,uint16_t psc);
void TIM_SetTIM1Compare2(uint16_t compare);
#endif

