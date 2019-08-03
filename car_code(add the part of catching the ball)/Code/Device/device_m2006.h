#ifndef _DEVICE_M2006_H
#define _DEVICE_M2006_H
//包含所需要的板载资源头文件
#include "bsp_can.h"
//接口区 
//映射M2006发送的CAN端口句柄
#define  M2006CAN_HandleYypeDef CAN_HandleTypeDef
//映射M2006发送时的CAN参数配置
#define  M2006CAN_TxHeaderTypeDef CanTxMsgTypeDef
//将要发送的信息送入CAN口发送
#define  M2006CAN_AddTxMessage  HAL_CAN_AddTxMessage
//映射M2006接受时的CAN信息
#define M2006CAN_RX   CanRxMsgTypeDef*




//2006电机反馈信息
typedef struct
{
    uint16_t angle;     //转子机械角度
    int16_t speed;      //转子速度
    uint16_t current;   //实际电流
    uint8_t temp;       //电机温度
    uint32_t fdbktime;  //反馈时间
    uint32_t count;     //累计收包数
    int32_t sum_angle;  //累计角度，最多行进6.5km，就会写爆
} M2006_FDBK_UNIT, *P_M2006_FDBK_UNIT;

//单条CAN总线上最多可挂载的M2006设备
typedef struct
{
    M2006_FDBK_UNIT m1;//1号电机
    M2006_FDBK_UNIT m2;//2号电机
    M2006_FDBK_UNIT m3;//3号电机
    M2006_FDBK_UNIT m4;//4号电机
    M2006_FDBK_UNIT m5;//5号电机
    M2006_FDBK_UNIT m6;//6号电机
    M2006_FDBK_UNIT m7;//7号电机
    M2006_FDBK_UNIT m8;//8号电机
} M2006_FDBK, *P_M2006_FDBK;

//M2006包的枚举类型，四个为一组
typedef enum
{
    M2006_PACK_1_4,//1-4号电机
    M2006_PACK_5_8,//5-8号电机
} M2006_PACK;

//M2006控制结构体
typedef struct
{
    int16_t setcur[4]; //4个电机的控制电流
    M2006_PACK pack;//哪四个电机
} M2006_CTRL, *P_M2006_CTRL;


void m2006SetCurrent(P_M2006_CTRL pctrl,M2006CAN_HandleYypeDef *hcan);
void m2006ResetFeedBack(P_M2006_FDBK pfdbk);
uint8_t m2006FeedBack(CAN_HandleTypeDef *pcan, P_M2006_FDBK pfdbk);

#endif



