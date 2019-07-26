#include "bsp_gpio.h"
//PB0推挽输出初始化
void GPIOPB0_Init(void)
{
	//GPIO初始化结构体
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOB_CLK_ENABLE(); //开启GPIOB时钟
	GPIO_Initure.Pin=GPIO_PIN_0; //PB0
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP; //推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP; //上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH; //高速
	//GPIO初始化
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET); //PB0置1
}
//PB1推挽输出初始化
void GPIOPB1_Init(void)
{
	//GPIO初始化结构体
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOB_CLK_ENABLE(); //开启GPIOB时钟
	GPIO_Initure.Pin=GPIO_PIN_1; //PB1
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP; //推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP; //上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH; //高速
	//GPIO初始化
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET); //PB1置1
}	



