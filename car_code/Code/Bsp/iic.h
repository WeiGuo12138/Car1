#ifndef __IIC_H
#define __IIC_H

#include "stm32f1xx_hal.h"
#include "delay.h"

#define IIC_SCL_GPIOx           GPIOC
#define IIC_SCL_GPIO_Pin        GPIO_PIN_0


#define IIC_SDA_GPIOx           GPIOC
#define IIC_SDA_GPIO_Pin        GPIO_PIN_1

#define IIC_SCL_HIGH()        HAL_GPIO_WritePin(IIC_SCL_GPIOx, IIC_SCL_GPIO_Pin,GPIO_PIN_SET)  //GPIO_SetBits(IIC_SCL_GPIOx, IIC_SCL_GPIO_Pin)
#define IIC_SCL_LOW()         HAL_GPIO_WritePin(IIC_SCL_GPIOx, IIC_SCL_GPIO_Pin,GPIO_PIN_RESET)  //GPIO_ResetBits(IIC_SCL_GPIOx, IIC_SCL_GPIO_Pin)
#define IIC_SDA_HIGH()        HAL_GPIO_WritePin(IIC_SDA_GPIOx, IIC_SDA_GPIO_Pin,GPIO_PIN_SET)  //GPIO_SetBits(IIC_SDA_GPIOx, IIC_SDA_GPIO_Pin)
#define IIC_SDA_LOW()         HAL_GPIO_WritePin(IIC_SDA_GPIOx, IIC_SDA_GPIO_Pin,GPIO_PIN_RESET)  //GPIO_ResetBits(IIC_SDA_GPIOx, IIC_SDA_GPIO_Pin)
#define IIC_SDA_READ()        HAL_GPIO_ReadPin(IIC_SDA_GPIOx, IIC_SDA_GPIO_Pin)  //GPIO_ReadInputDataBit(IIC_SDA_GPIOx, IIC_SDA_GPIO_Pin)

//IIC³õÊ¼»¯º¯Êý  
void IIC_Init(void); 
  
unsigned char IIC_Write(unsigned char addr, unsigned char reg, unsigned char len, unsigned char * data);  
unsigned char IIC_Read(unsigned char addr, unsigned char reg, unsigned char len, unsigned char * buf);  

  
#endif  
  

