#include "tim.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103开发板
//定时器中断驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/5/25
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
TIM_HandleTypeDef 	TIM1_Handler;      	//定时器句柄 
TIM_OC_InitTypeDef 	TIM1_CH1Handler;		//定时器3通道2句柄

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM1_Init(uint16_t arr,uint16_t psc)
{  
    TIM1_Handler.Instance=TIM1;                          //通用定时器3
    TIM1_Handler.Init.Prescaler=psc;                     //分频系数
    TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM1_Handler.Init.Period=arr;                        //自动装载值
    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM1_Handler);
    HAL_TIM_Base_Start_IT(&TIM1_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
}

//TIM3 PWM部分初始化 
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
void TIM1_PWM_Init(uint16_t arr,uint16_t psc)
{  
    TIM1_Handler.Instance=TIM1;          	//定时器3
    TIM1_Handler.Init.Prescaler=psc;       //定时器分频
    TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM1_Handler.Init.Period=arr;          //自动重装载值
    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM1_Handler);       //初始化PWM
    
    TIM1_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM1_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM1_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM1_Handler,&TIM1_CH1Handler,TIM_CHANNEL_1);//配置TIM3通道2
	
    HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_1);//开启PWM通道2
	 	   
}

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM1_UP_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn );          //开启ITM3中断     //这个也不太一样
	}
}

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	
    if(htim->Instance==TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();			//使能定时器3
//		__HAL_AFIO_REMAP_TIM4_PARTIAL();		//TIM3通道引脚部分重映射使能
		__HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOB时钟
		
		GPIO_Initure.Pin=GPIO_PIN_8;           	//PB5
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH ;     //高速  //这个有点不太一样
		HAL_GPIO_Init(GPIOA,&GPIO_Initure); 	
	}
}

//设置TIM通道2的占空比
//compare:比较值
void TIM_SetTIM1Compare2(uint16_t compare)
{
	TIM1->CCR1=compare; 
}

//定时器3中断服务函数
void TIM1_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM1_Handler);
}

//回调函数，定时器中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&TIM1_Handler))
    {
  
    }
}
