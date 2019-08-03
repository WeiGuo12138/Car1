#ifndef  _PROTOCAL_H
#define  _PROTOCAL_H

#include "usart.h"

//#define OpenmvUart huart2   //串口2 宏定义

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


} DEF_STATE;

//接收信息结构体
typedef struct
{
    DEF_STATE state;
    unsigned char Cmd_ID;
    unsigned char  Length;
    unsigned char Re_Data[20];
    unsigned char Check;
} DEF_RE_RECEIVE;

//
typedef struct
{
    unsigned char ready_op;
} DEF_READ;

extern uint8_t check[20];
extern int16_t pack_motor[2];
void RECEIVE_PACK(unsigned char data);

#endif
