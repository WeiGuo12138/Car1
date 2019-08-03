#ifndef _DEVICE_LED_H
#define _DEVICE_LED_H
//包含板载资源区
#include "bsp_gpio.h"
//包含芯片资源区
#include "core_cm3.h"

//亮灭标志
#define LED_ON     1
#define LED_OFF		 0

//设备接口区
#define LedRedGPIOInit  GPIOPB1_Init
#define LedGreenGPIOInit  GPIOPB0_Init

#define LEDWriteState					HAL_GPIO_WritePin
#define LEDToggleState        HAL_GPIO_TogglePin

#define LEDGreenGPIO 	GPIOB
#define LEDGreenPin		GPIO_PIN_0
#define LEDGreenON		GPIO_PIN_RESET
#define LEDGreenOFF		GPIO_PIN_SET

#define LEDRedGPIO 	GPIOB
#define LEDRedPin		GPIO_PIN_1
#define LEDRedON		GPIO_PIN_RESET
#define LEDRedOFF		GPIO_PIN_SET	


typedef struct
{
	uint8_t id;//序号
	union
	{
		uint8_t value;
		struct
		{
			uint8_t IsInited :1;//初始化标志位
			uint8_t LightState :1;//亮灭标志位
		}LED_STATE;
	}LED_ECHO;
}_DEVICE_LED;


void LED_Init(void);
void LED_On(_DEVICE_LED led);
void LED_Off(_DEVICE_LED led);
void LED_Toggle(_DEVICE_LED led);

 
extern _DEVICE_LED LED_RED;
extern _DEVICE_LED LED_GREEN;
#endif


