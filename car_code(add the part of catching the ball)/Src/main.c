#include "main.h"
#include "stm32f1xx_hal.h"
#include "bsp_gpio.h"
#include "device_led.h"
#include "bsp_can.h"
#include "usart.h"
#include "tim.h"
#include "device_m2006.h"
#include "module_chassis.h"
#include "spi.h"
#include "oled.h"
#include "delay.h"
#include "iic.h"
#include "adc.h"
#include "protocal.h"

uint8_t M2006_REV_TEST[8],CAR_TX_TEST[8]={0,0,0,0,1,10,0,0};

uint8_t laser_can1_data[8]={0};


extern  int16_t rcount[4];

extern uint16_t cnt;
uint8_t count=0;
uint8_t qr_count=0;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
extern uint8_t openmv_u2_can1[8];
extern uint8_t oled_u1_can1[8];
extern uint8_t openmv_color_flag;
extern uint8_t openmv_qr_flag;

uint8_t guess_color=0;
uint8_t guess_color2=0;

uint8_t pwmvalue = 0;

//主函数
int main(void)
{
//long int  t_ch = 0,t_oled = 0;
	//HAL初始化
    HAL_Init();
	//系统时钟初始化
    SystemClock_Config();
    MX_GPIO_Init();
	//MX_TIM1_Init();
	//LED初始化
	LED_Init();
	//CAN1初始化
	CAN1_Init();
	//CAN2初始化
	CAN2_Init();
	//USART2初始化
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
    //spi初始化
	MX_SPI1_Init();
	//oled初始化
    oled_init();  
	//开启CAN1总线的接收中断	
	 HAL_CAN_Receive_IT(&hcan1,CAN_FIFO0);
	//开启CAN2总线的接收中断	
	HAL_CAN_Receive_IT(&hcan2,CAN_FIFO0);
//	HAL_TIM_Base_Start_IT(&htim1);
    MX_ADC1_Init();
    TIM1_PWM_Init(200-1,7200-1);
    HAL_Delay(5000);//延时5s
    while (1)
    {
      Chassis();//底盘的  
      //  printf("1");
       if(pack_motor[0]==1)
       {
           TIM_SetTIM1Compare2(170);
           printf("OK");
       }
       if(pack_motor[0]==2)
       {
           TIM_SetTIM1Compare2(190);
       }
       
	}
  
}

//系统时钟初始化
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.Prediv1Source = RCC_PREDIV1_SOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /**Configure the Systick interrupt time 
    */
  __HAL_RCC_PLLI2S_ENABLE();

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}




 void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  GPIO_InitTypeDef GPIO_InitStruct = {0};


  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, OLED_DC_Pin|OLED_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = OLED_DC_Pin|OLED_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


}

//错误中断函数
void _Error_Handler(char *file, int line)
{
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 

}
#endif /* USE_FULL_ASSERT */

