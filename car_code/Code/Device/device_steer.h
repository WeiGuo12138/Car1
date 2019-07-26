#ifndef _DEVICE_STEER_H
#define _DEVICE_STEER_H

#include "stm32f1xx_hal.h"
#include "bsp_usart.h"
#include "string.h"

#define online 1
#define offline 2

#define ArmUart huart3   //串口宏定义，方便移植

#define MotorMode 1
#define ServoMode 2

typedef struct
{
	uint8_t ID;//舵机ID
	uint8_t mode;
  int16_t TagAngle;//舵机目标角度
	int16_t Speed;//舵机转动目标速度
	uint16_t TagAngleDelayTime;//舵机转动到目标角度所用的时间
	int16_t ActAngle;//舵机实时角度
	int16_t LastTagAngle;//舵机初始角度
}SERVO_HandleTypeDef;//舵机句柄

extern uint8_t ArmRxDatas[10];

void SERVO_MOVE_TIME_WRITE(SERVO_HandleTypeDef *Arm_Write);
void SERVO_MOVE_TIME_READ(SERVO_HandleTypeDef *ServoWrite);
void SERVO_ANGLE_OFFSET_ADJUST(SERVO_HandleTypeDef *ServoWrite);
void SERVO_POS_READ(SERVO_HandleTypeDef *Servo);
void SERVO_OR_MOTOR_MODE_WRITE(SERVO_HandleTypeDef *Servo);
void ServoDriver(SERVO_HandleTypeDef *Servo);
void MotorRemoteTest(SERVO_HandleTypeDef *Servo,int16_t ch);

#endif
