#ifndef _BSP_CAN_H
#define _BSP_CAN_H

#include "stm32f1xx_hal.h"

#define DATA_LEN 8
//CAN2

void CAN2_Init(void);
extern CAN_HandleTypeDef hcan2;
extern uint8_t Can2RxDatas[2];
//CAN1
void CAN1_Init(void);
extern CAN_HandleTypeDef hcan1;
extern uint8_t Can1RxDatas[2];
//CAN2
uint8_t Can2SendMsg(uint8_t* msg,CanTxMsgTypeDef txstate);
void Can2RecDatas(uint8_t *msg,uint32_t FIFONumber);
//CAN1
uint8_t Can1SendMsg(uint8_t* msg,CanTxMsgTypeDef txstate);
uint16_t Can1RecDatas(uint8_t *msg,uint32_t FIFONumber);
uint8_t Can1SendMsg_TEST(uint8_t* msg,uint16_t device_id);
#endif




