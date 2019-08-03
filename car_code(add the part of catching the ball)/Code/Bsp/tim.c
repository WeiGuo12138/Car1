#include "tim.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F103������
//��ʱ���ж���������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/5/25
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
TIM_HandleTypeDef 	TIM1_Handler;      	//��ʱ����� 
TIM_OC_InitTypeDef 	TIM1_CH1Handler;		//��ʱ��3ͨ��2���

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM1_Init(uint16_t arr,uint16_t psc)
{  
    TIM1_Handler.Instance=TIM1;                          //ͨ�ö�ʱ��3
    TIM1_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM1_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM1_Handler);
    HAL_TIM_Base_Start_IT(&TIM1_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
}

//TIM3 PWM���ֳ�ʼ�� 
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
void TIM1_PWM_Init(uint16_t arr,uint16_t psc)
{  
    TIM1_Handler.Instance=TIM1;          	//��ʱ��3
    TIM1_Handler.Init.Prescaler=psc;       //��ʱ����Ƶ
    TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���ģʽ
    TIM1_Handler.Init.Period=arr;          //�Զ���װ��ֵ
    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM1_Handler);       //��ʼ��PWM
    
    TIM1_CH1Handler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIM1_CH1Handler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM1_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; //����Ƚϼ���Ϊ�� 
    HAL_TIM_PWM_ConfigChannel(&TIM1_Handler,&TIM1_CH1Handler,TIM_CHANNEL_1);//����TIM3ͨ��2
	
    HAL_TIM_PWM_Start(&TIM1_Handler,TIM_CHANNEL_1);//����PWMͨ��2
	 	   
}

//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();            //ʹ��TIM3ʱ��
		HAL_NVIC_SetPriority(TIM1_UP_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn );          //����ITM3�ж�     //���Ҳ��̫һ��
	}
}

//��ʱ���ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_PWM_Init()����
//htim:��ʱ�����
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	
    if(htim->Instance==TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();			//ʹ�ܶ�ʱ��3
//		__HAL_AFIO_REMAP_TIM4_PARTIAL();		//TIM3ͨ�����Ų�����ӳ��ʹ��
		__HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOBʱ��
		
		GPIO_Initure.Pin=GPIO_PIN_8;           	//PB5
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;          //����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH ;     //����  //����е㲻̫һ��
		HAL_GPIO_Init(GPIOA,&GPIO_Initure); 	
	}
}

//����TIMͨ��2��ռ�ձ�
//compare:�Ƚ�ֵ
void TIM_SetTIM1Compare2(uint16_t compare)
{
	TIM1->CCR1=compare; 
}

//��ʱ��3�жϷ�����
void TIM1_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM1_Handler);
}

//�ص���������ʱ���жϷ���������
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&TIM1_Handler))
    {
  
    }
}
