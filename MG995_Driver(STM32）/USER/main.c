#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "protocol.h"


int main(void)
{
//	u8 dir=1;
    u8 pwmval=0;

    HAL_Init();                    	 	//初始化HAL库
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
    delay_init(72);               		//初始化延时函数
    uart_init(115200);					//初始化串口
    LED_Init();							//初始化LED
//	KEY_Init();							//初始化按键
    TIM4_PWM_Init(200-1,7200-1);       	//72M/72=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
    while(1)
    {
        delay_ms(10);
        if(pack_motor[0]==1)
        {
            pwmval=175;
          //  TIM_SetTIM3Compare2(175);
          // printf("1");
        }
        if(pack_motor[0]==2)
        {
            pwmval=195;
         //   TIM_SetTIM3Compare2(195);
        }
         TIM_SetTIM3Compare2(pwmval);
//      最大最好不要超过195 最小不要低于175
//        printf("hello world/n");
//        HAL_Delay(200);
//        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);	//灯灭
//        HAL_Delay(200);
//         HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);	//灯亮
//        HAL_Delay(200);
    }
}
