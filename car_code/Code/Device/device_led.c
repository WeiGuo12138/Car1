#include "device_led.h"

//红色LED句柄
_DEVICE_LED LED_RED = {0};
//绿色LED句柄
_DEVICE_LED LED_GREEN = {0};

//LED设备初始化
void LED_Init(void)
{
	//红色LEDid为1
	LED_RED.id = 1;
	//红色LEDGPIO初始化
	LedRedGPIOInit();
	//初始化标志位置1
	LED_RED.LED_ECHO.LED_STATE.IsInited = 1;
	
	//绿色LEDid为2
	LED_GREEN.id = 2;
	//绿色LEDGPIO初始化
	LedGreenGPIOInit();
	//初始化标志位置1
	LED_GREEN.LED_ECHO.LED_STATE.IsInited = 1;
}

//点亮LED
void LED_On(_DEVICE_LED led)
{
	//点亮标志位置一
	led.LED_ECHO.LED_STATE.LightState = LED_ON;
	//根据ID号判断点亮那个led
	switch(led.id)
	{
		//打开一号灯
		case 1:
			LEDWriteState(LEDRedGPIO,LEDRedPin,LEDRedON);
			break;
		//打开二号灯
		case 2:
			LEDWriteState(LEDGreenGPIO,LEDGreenPin,LEDGreenON);
			break;
		//ID错误
		default:break;
	}
}

//熄灭LED
void LED_Off(_DEVICE_LED led)
{
	//点亮标志位置0
	led.LED_ECHO.LED_STATE.LightState = LED_OFF;
	//根据ID号判断熄灭那个led
	switch(led.id)
	{
		//熄灭一号灯
		case 1:
			LEDWriteState(LEDRedGPIO,LEDRedPin,LEDRedOFF);
			break;
		//熄灭二号灯
		case 2:
			LEDWriteState(LEDGreenGPIO,LEDGreenPin,LEDGreenOFF);
			break;
		//ID错误
		default:break;
	}
}

//反转LED
void LED_Toggle(_DEVICE_LED led)
{
	//点亮标志位
	//如果之前的标志位为关闭
	if(led.LED_ECHO.LED_STATE.LightState == LED_OFF)
	{
		//点亮标志位置1
		led.LED_ECHO.LED_STATE.LightState = LED_ON;
	}
	else
	{
		//点亮标志位置0
		led.LED_ECHO.LED_STATE.LightState = LED_OFF;
	}
	
	//根据ID号判断反转那个led
	switch(led.id)
	{
		//反转一号灯
		case 1:
			LEDToggleState(LEDRedGPIO,LEDRedPin);
			break;
		//反转二号灯
		case 2:
			LEDToggleState(LEDGreenGPIO,LEDGreenPin);
			break;
		//ID错误
		default:break;
	}
}
