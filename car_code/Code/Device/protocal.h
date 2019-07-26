#ifndef  _PROTOCAL_H
#define  _PROTOCAL_H

#include "bsp_usart.h"

#define OpenmvUart huart2   //串口2 宏定义

typedef enum
{
		//包头1
    HEADER1,
	  //包头2
	  HEADER2,
		//ID	
    CMDID1,
		//长度位
    LENGTH1,
		//数据位
    DATA1,
	  //校验位
    CHECK1,
	  //包尾
	  END,
	
	  //包头1
    //HEADER1,
	  //包头2
	  //HEADER2,
		//ID	
    CMDID2,
		//长度位
    LENGTH2,
		//数据位
    DATA2,
	  //校验位
    CHECK2
	  //包尾
	
}DEF_STATE;

//接收信息结构体
typedef struct
{
		DEF_STATE state;
    unsigned char Cmd_ID;
    unsigned char  Length;
    unsigned char Re_Data[20];
    unsigned char Check;
}DEF_RE_RECEIVE;

//
typedef struct
{
	unsigned char ready_op;
}DEF_READ;

extern uint8_t check[20];
extern int16_t pack_velocity[4];
extern int16_t pack_imu[4];
void RECEIVE_PACK(unsigned char data);

#endif
