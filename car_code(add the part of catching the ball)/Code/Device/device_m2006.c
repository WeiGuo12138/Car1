#include "device_m2006.h"

//M2006转动圈数计数
int16_t rcount[4]={0};

//给2006电机发送电流
void m2006SetCurrent(P_M2006_CTRL pctrl,M2006CAN_HandleYypeDef *hcan)
{
    //第一组2006的CAN缓冲区
    static uint8_t m2006_p1data[8];
    //第二组2006的CAN缓冲区
    static uint8_t m2006_p2data[8];

    //第一组2006发送的CAN配置
    static M2006CAN_TxHeaderTypeDef M2006_PACK1_HDR  =
    {
        //标准地址
        0x200,
        //扩展地址
        0,
        //标准ID还是拓展ID
        CAN_ID_STD,
        //标准帧还是远程帧
        CAN_RTR_DATA,
        //将要发送的数据的长度（字节）
        8
    };

    //第二组2006发送的CAN配置
    static M2006CAN_TxHeaderTypeDef M2006_PACK2_HDR  =
    {
        //标准地址
        0x1FF,
        //扩展地址
        0,
        //标准ID还是拓展ID
        CAN_ID_STD,
        //标准帧还是远程帧
        CAN_RTR_DATA,
        //将要发送的数据的长度（字节）
        8
    };

    //用于做循环计数
    int i;
    //如果是第1组电机
    if(pctrl->pack == M2006_PACK_1_4)
    {
        //进行循环赋值，每个电流占两个字节
        for(i = 0; i <= 3; i++)    
        {
            //高位在前
            m2006_p1data[i * 2]         =   pctrl->setcur[i] >> 8;
            //低位在后
            m2006_p1data[i * 2 + 1]     =   pctrl->setcur[i] & 0x00ff;
        }
				Can2SendMsg(m2006_p1data,M2006_PACK1_HDR);
		}
    //如果是第2组电机
    else
    {
        //进行循环赋值，每个电流占两个字节
        for(i = 0; i <= 3; i++)    
        {
            //高位在前
            m2006_p2data[i * 2]         =   pctrl->setcur[i] >> 8;
            //低位在后
            m2006_p2data[i * 2 + 1]     =   pctrl->setcur[i] & 0x00ff;
        }
				Can2SendMsg(m2006_p2data,M2006_PACK2_HDR);
    }
}

//复位2006电机的反馈数据
void m2006ResetFeedBack(P_M2006_FDBK pfdbk)
{
    //映射首地址
    P_M2006_FDBK_UNIT pfirst = &pfdbk->m1;
    //用于做循环计数
    int i;
    //对所有电机参数进行循环复位
    for(i = 0; i <= 7; i++)
    {
        //复位反馈时间
        pfirst[i].fdbktime      = 0;
        //复位反馈机械角度
        pfirst[i].angle         = 0;
        //复位反馈速度
        pfirst[i].speed         = 0;
        //复位反馈实时电流
        pfirst[i].current       = 0;
        //复位反馈温度
        pfirst[i].temp          = 0;
        //复位反馈收包数   
        pfirst[i].count         = 0;
        //复位反馈累计角度
        pfirst[i].sum_angle     = 0;
    }
}

//复位2006电机的记圈数据
void m2006ResetRoundcnt(P_M2006_FDBK pfdbk)
{
    //映射首地址
    P_M2006_FDBK_UNIT pfirst = &pfdbk->m1;
    //用于做循环计数
    int i;
    //对所有电机参数进行循环复位
    for(i = 0; i <= 7; i++)
    {
        //复位反馈累计角度
        pfirst[i].sum_angle     = 0;
    }
		for(i=0;i<=4;i++)
			rcount[i]=0;
}

//m2006反馈接受
uint8_t m2006FeedBack(CAN_HandleTypeDef *pcan, P_M2006_FDBK pfdbk)
{
	uint8_t can_rev[8] = {0};
	uint8_t i;
	//选定FIFO
	pcan->pRxMsg->FIFONumber=CAN_FIFO0;
  //判断电机ID
  uint16_t id = pcan->pRxMsg->StdId - 0x200;
	//映射首地址
	P_M2006_FDBK_UNIT pfirst    = &pfdbk->m1;
	uint16_t last_angle = pfirst[id - 1].angle;  //实际角度
	//接收数据
	for(i=0;i<8;i++)
	{
		//接收数据
		can_rev[i]=pcan->pRxMsg->Data[i];
		//清空FIFO0
		CAN1->RF0R |= CAN_RF0R_RFOM0;//清空FIFO0
	}
	//此处未写获得反馈时间的接收
	//反馈角度
	pfirst[id - 1].angle        = (can_rev[0] << 8) + can_rev[1];
	//反馈速度
	pfirst[id - 1].speed        = (can_rev[2] << 8) + can_rev[3];
	//反馈电流
	pfirst[id - 1].current      = (can_rev[4] << 8) + can_rev[5];
	//反馈温度
	pfirst[id - 1].temp         = can_rev[6];                          //没用到
	//反馈计数
	pfirst[id - 1].count        = pfirst[id - 1].count + 1;            
	//此处未写获得总里程计数
		if(pfirst[id - 1].count == 1)  //电机刚开始工作，转的角度为0
    {
        pfirst[id - 1].sum_angle = 0;
    }
    else
    {
        pfirst[id - 1].sum_angle += ((double)(pfirst[id - 1].angle) - (double)last_angle);
        if( (int)(pfirst[id - 1].angle) - (int)last_angle > 5000 )          //反向转一圈
        {
            pfirst[id - 1].sum_angle -= 8192;
        }
        else if( (int)(pfirst[id - 1].angle) - (int)last_angle < -5000 )    //正向转一圈  
        {
            pfirst[id - 1].sum_angle += 8192;   // 1 = 0.04395
        }
			}
		//计数电机转动的圈数
		rcount[id-1]=pfirst[id - 1].sum_angle/8192;
			
	//返回ID号
	return id;
}


