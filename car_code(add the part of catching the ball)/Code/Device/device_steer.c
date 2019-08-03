
#include "device_steer.h"

uint8_t ServoTx[10]={0x55,0x55,0x01,0x07,0x01,0x00,0x00,0x00,0x00,0x00};
uint8_t ServoRx[10];
uint8_t ArmRxDatas[10];

void SERVO_MOVE_TIME_WRITE(SERVO_HandleTypeDef *Servo)
{
	uint8_t count; 
	uint16_t check=0;
	ServoTx[2]=Servo->ID;
	ServoTx[3]=0x07;
	ServoTx[4]=0x01;
	ServoTx[5]=(uint16_t)Servo->TagAngle&0xff;
	ServoTx[6]=(uint16_t)Servo->TagAngle>>8;
	ServoTx[7]=Servo->TagAngleDelayTime&0xff;
	ServoTx[8]=Servo->TagAngleDelayTime>>8;
//	ServoTx[7]=Servo->TagAngleDelayTime=0;
//	ServoTx[8]=Servo->TagAngleDelayTime=0;
	for(count=0;count<7;count++){
			check+=ServoTx[count+2];
	}
	check%=256;
	check=~check;
	ServoTx[9]=(uint8_t)check;
	while(HAL_UART_Transmit(&ArmUart,ServoTx, 10, 0xffff)!=HAL_OK);
}

void SERVO_POS_READ(SERVO_HandleTypeDef *Servo)
{
	uint8_t count;
	int16_t temp; 
	uint16_t check=0;
	ServoTx[2]=Servo->ID;
	ServoTx[3]=0x03;
	ServoTx[4]=0x1c;
	for(count=0;count<3;count++){
			check+=ServoTx[count+2];
	}
	check%=256;
	check=~check;
	ServoTx[5]=(uint8_t)check;
	HAL_UART_Transmit(&ArmUart,ServoTx, 6, 0xffff);
	HAL_UART_Receive(&ArmUart,ServoRx,8,0x01);
	temp=ServoRx[6]*256+ServoRx[5];//读取舵机实际角度值
	if(ServoRx[2]==Servo->ID) Servo->ActAngle=temp;
}

void SERVO_OR_MOTOR_MODE_WRITE(SERVO_HandleTypeDef *Servo)
{
  uint8_t count; 
	uint16_t check=0;
	ServoTx[2]=Servo->ID;
	ServoTx[3]=0x07;
	ServoTx[4]=0x1d;
	ServoTx[5]=0x01;
	if(Servo->mode==ServoMode)
	  ServoTx[6]=0;
	else
		ServoTx[6]=1;
	ServoTx[7]=(int16_t)Servo->Speed&0xff;
	ServoTx[8]=(int16_t)Servo->Speed>>8;
	for(count=0;count<7;count++){
			check+=ServoTx[count+2];
	}
	check%=256;
	check=~check;
	ServoTx[9]=(uint8_t)check;
	HAL_UART_Transmit(&ArmUart,ServoTx, 10, 0xffff);
}

void ServoDriver(SERVO_HandleTypeDef *Servo)
{
	if(Servo->mode==MotorMode)
	  SERVO_OR_MOTOR_MODE_WRITE(Servo);
	else
	{
//		SERVO_OR_MOTOR_MODE_WRITE(Servo);
		SERVO_MOVE_TIME_WRITE(Servo);
	}
}


