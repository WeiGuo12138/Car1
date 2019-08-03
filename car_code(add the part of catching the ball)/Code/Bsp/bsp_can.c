#include "bsp_can.h"
//CAN2句柄
CAN_HandleTypeDef hcan2;
CAN_HandleTypeDef hcan1;
//CAN2过滤器句柄
CAN_FilterConfTypeDef CAN2_Filter;
//CAN1过滤器句柄
CAN_FilterConfTypeDef CAN1_Filter;
//CAN2发送邮箱句柄
static CanTxMsgTypeDef        Can2TxMessage;
//CAN1发送邮箱句柄
static CanTxMsgTypeDef        Can1TxMessage;
//CAN2接收邮箱句柄
static CanRxMsgTypeDef        Can2RxMessage;
//CAN1接收邮箱句柄
static CanRxMsgTypeDef        Can1RxMessage;
//初始化CAN2

void CAN1_Init(void)
{
	hcan1.pTxMsg = &Can1TxMessage;
  hcan1.pRxMsg = &Can1RxMessage;
	//CAN端口
  hcan1.Instance = CAN1;
	//3分频
  hcan1.Init.Prescaler = 3;
	//回环模式
  hcan1.Init.Mode = CAN_MODE_NORMAL;
	//可跳跃时间段为4TQ
  hcan1.Init.SJW = CAN_SJW_4TQ;
	//BS1时间段为6TQ
  hcan1.Init.BS1 = CAN_BS1_6TQ;
	//BS2时间段为5TQ
  hcan1.Init.BS2 = CAN_BS2_5TQ;
	//非时间触发通信模式关闭
  hcan1.Init.TTCM = DISABLE;
	//软件自动离线管理关闭
  hcan1.Init.ABOM = DISABLE;
	//睡眠模式通过软件唤醒模式关闭
  hcan1.Init.AWUM = DISABLE;
	//禁止报文自动传送开启
  hcan1.Init.NART = ENABLE;
	//报文不锁定，新的覆盖旧的关闭
  hcan1.Init.RFLM = DISABLE;
	//优先级由报文标识符决定关闭
  hcan1.Init.TXFP = DISABLE;
	//初始化CAN1
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	//激活过滤器
	CAN1_Filter.FilterActivation=ENABLE;
	//过滤器0关联到FIFO0
	CAN1_Filter.FilterFIFOAssignment=CAN_FILTER_FIFO0;
	//期望收到的第一个ID
	CAN1_Filter.FilterIdHigh=0x0000;
	//期望收到的弟二个ID
	CAN1_Filter.FilterIdLow=0x0000;
	//期望收到的第三个ID
	CAN1_Filter.FilterMaskIdHigh=0;
	//期望收到的第四个ID
	CAN1_Filter.FilterMaskIdLow=0;
	//标识符屏蔽模式
	CAN1_Filter.FilterMode=CAN_FILTERMODE_IDMASK;
	//选择过滤器0
	CAN1_Filter.FilterNumber=0;
	//设置为32位模式
	CAN1_Filter.FilterScale=CAN_FILTERSCALE_32BIT;
	
	CAN1_Filter.BankNumber=0;
	//初始化过滤器，can1和can2共用过滤器
	HAL_CAN_ConfigFilter(&hcan1,&CAN1_Filter);
}

void CAN2_Init(void)
{
	hcan2.pTxMsg = &Can2TxMessage;
  hcan2.pRxMsg = &Can2RxMessage;
	//CAN端口
  hcan2.Instance = CAN2;
	//3分频
  hcan2.Init.Prescaler = 3;
	//普通模式
  hcan2.Init.Mode = CAN_MODE_NORMAL;
	//可跳跃时间段为4TQ
  hcan2.Init.SJW = CAN_SJW_4TQ;
	//BS1时间段为6TQ
  hcan2.Init.BS1 = CAN_BS1_6TQ;
	//BS2时间段为5TQ
  hcan2.Init.BS2 = CAN_BS2_5TQ;
	//非时间触发通信模式关闭
  hcan2.Init.TTCM = DISABLE;
	//软件自动离线管理关闭
  hcan2.Init.ABOM = DISABLE;
	//睡眠模式通过软件唤醒模式关闭
  hcan2.Init.AWUM = DISABLE;
	//禁止报文自动传送开启
  hcan2.Init.NART = ENABLE;
	//报文不锁定，新的覆盖旧的关闭
  hcan2.Init.RFLM = DISABLE;
	//优先级由报文标识符决定关闭
  hcan2.Init.TXFP = DISABLE;
	//初始化CAN2
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	//激活过滤器
	CAN2_Filter.FilterActivation=ENABLE;
	//过滤器0关联到FIFO0
	CAN2_Filter.FilterFIFOAssignment=CAN_FILTER_FIFO0;
	//期望收到的第一个ID
	CAN2_Filter.FilterIdHigh=0x0000;
	//期望收到的弟二个ID
	CAN2_Filter.FilterIdLow=0x0000;
	//期望收到的第三个ID
	CAN2_Filter.FilterMaskIdHigh=0;
	//期望收到的第四个ID
	CAN2_Filter.FilterMaskIdLow=0;
	//标识符屏蔽模式
	CAN2_Filter.FilterMode=CAN_FILTERMODE_IDMASK;
	//选择过滤器0
	CAN2_Filter.FilterNumber=0;
	//设置为32位模式
	CAN2_Filter.FilterScale=CAN_FILTERSCALE_32BIT;
	
	CAN2_Filter.BankNumber=0;
	//初始化过滤器，can1和can2共用过滤器
	HAL_CAN_ConfigFilter(&hcan1,&CAN2_Filter);
}

void Can2RecDatas(uint8_t *msg,uint32_t FIFONumber)
{
	uint8_t i;
	//选定FIFO
	hcan2.pRxMsg->FIFONumber=FIFONumber;
	//接收数据
	for(i=0;i<DATA_LEN;i++)
	{
		//接收数据
		msg[i]=hcan2.pRxMsg->Data[i];
		//清空FIFO
		if (FIFONumber == CAN_FIFO0)
		{
			//清空FIFO0
			CAN2->RF0R |= CAN_RF0R_RFOM0;//清空FIFO0
		}
		else 
		{
			//清空FIFO1
			CAN2->RF1R |= CAN_RF1R_RFOM1;//清空FIFO1
		}
	}
}

//CAN2发送函数
uint8_t Can2SendMsg(uint8_t* msg,CanTxMsgTypeDef txstate)
{	
	//计数器
	uint16_t i=0;
	//确定发送参数标准标识符还是延长标识符
	hcan2.pTxMsg->StdId=txstate.StdId;			
	//确定发送参数标准帧	
	hcan2.pTxMsg->IDE=txstate.IDE;
	//确定发送参数数据帧还是远程控制帧
	hcan2.pTxMsg->RTR=txstate.RTR;
	//确定发送参数数据的长度
	hcan2.pTxMsg->DLC=txstate.DLC;	
	//向邮箱填充数据
	for(i=0;i<txstate.DLC;i++)
	  hcan2.pTxMsg->Data[i]=msg[i];	 //发送数据
	while(1)
	//发送数据
	if(HAL_CAN_Transmit(&hcan2,10)==HAL_OK)
	break;	//发送数据
	i=0; 
	return 0;	 
}

uint16_t Can1RecDatas(uint8_t *msg,uint32_t FIFONumber)
{
	uint8_t i;
	//选定FIFO
	hcan1.pRxMsg->FIFONumber=FIFONumber;
	//接收数据
	for(i=0;i<DATA_LEN;i++)
	{
		//接收数据
		msg[i]=hcan1.pRxMsg->Data[i];
		//清空FIFO
		if (FIFONumber == CAN_FIFO0)
		{
			//清空FIFO0
			CAN1->RF0R |= CAN_RF0R_RFOM0;//清空FIFO0
		}
		else 
		{
			//清空FIFO1
			CAN1->RF1R |= CAN_RF1R_RFOM1;//清空FIFO1
		}
	}
	return hcan1.pRxMsg->StdId;
}

//CAN2发送函数
uint8_t Can1SendMsg(uint8_t* msg,CanTxMsgTypeDef txstate)
{	
	//计数器
	uint16_t i=0;
	//确定发送参数标准标识符还是延长标识符
	hcan1.pTxMsg->StdId=txstate.StdId;			
	//确定发送参数标准帧	
	hcan1.pTxMsg->IDE=txstate.IDE;
	//确定发送参数数据帧还是远程控制帧
	hcan1.pTxMsg->RTR=txstate.RTR;
	//确定发送参数数据的长度
	hcan1.pTxMsg->DLC=txstate.DLC;	
	//向邮箱填充数据
	for(i=0;i<txstate.DLC;i++)
	  hcan1.pTxMsg->Data[i]=msg[i];	 //发送数据
	while(1)
	//发送数据
	if(HAL_CAN_Transmit(&hcan1,10)==HAL_OK)
	break;	//发送数据
	i=0; 
	return 0;	 
}

uint8_t Can1SendMsg_TEST(uint8_t* msg,uint16_t device_id)
{	
	//计数器
	uint16_t i=0;
	//确定发送参数标准标识符还是延长标识符
	hcan1.pTxMsg->StdId=device_id;			
	//确定发送参数标准帧	
	hcan1.pTxMsg->IDE=CAN_ID_STD;
	//确定发送参数数据帧还是远程控制帧
	hcan1.pTxMsg->RTR=CAN_RTR_DATA;
	//确定发送参数数据的长度
	hcan1.pTxMsg->DLC=8;	
	//向邮箱填充数据
	for(i=0;i<8;i++)
	  hcan1.pTxMsg->Data[i]=msg[i];	 //发送数据
	while(1)
	//发送数据
	if(HAL_CAN_Transmit(&hcan1,10)==HAL_OK)
	break;	//发送数据
	i=0; 
	return 0;	 
}
