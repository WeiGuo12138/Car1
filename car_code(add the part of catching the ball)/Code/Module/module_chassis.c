#include "module_chassis.h"
#include "protocal.h"
#include "usart.h"

#define L 100;      //单位是mm
//PID初始化函数申明
void Pid_Init(void);
//M2006反馈句柄（单条总线上能挂载电机的总和）
M2006_FDBK m2006 = {0};
//速度环PID参数
PID_HANDLE pid_speed = {0};
//一号轮PID参数
PID_HANDLE s1_speed = {0};
//二号轮PID参数
PID_HANDLE s2_speed = {0};
//三号轮PID参数
PID_HANDLE s3_speed = {0};
//四号轮PID参数
PID_HANDLE s4_speed = {0};
//imu数据pid参数
PID_HANDLE imu_data = {0};
//2006电机控制句柄
M2006_CTRL  m2006ctrl = {0};
//底盘控制
CAR Car={0,0,0};

//轮子转动圈数
extern  int16_t rcount[4];

//小车控制
void Chassis(void)
{
	uint8_t msg[8];
	//输出给一号轮的电流
	double output_current1 = 0;
	//输出给二号轮的电流
	double output_current2 = 0;
	//输出给三号轮的电流
	double output_current3 = 0;
	//输出给四号轮的电流
	double output_current4 = 0;
	//四个轮子的速度
	double vw1, vw2, vw3, vw4, imu;
	
	//赋值 vx vy w
	Car.vx = pack_velocity[0];
	Car.vy = pack_velocity[1];
	Car.w  = pack_velocity[2];
	
	//pid初始化
	Pid_Init();
	//对于imu信息的pid处理
	//imu = imu_pid(&imu_data, pack_imu[0]);
	//printf("imudata: %lf\r\n", imu);
   //全向轮解算
	 vw1 = + Car.vy - Car.w /10 ;                            //轮子逆时针为正
	 vw2 =   0.866 * Car.vx - 0.5 * Car.vy - Car.w / 10 ;
	 vw3 = - 0.866 * Car.vx - 0.5 * Car.vy - Car.w / 10 ;
	 vw4 = 0; 
	  
		
		//一号轮速度PID控制
		output_current1 = pidProcess(&s1_speed, vw1, m2006.m1.speed);
		//二号轮速度PID控制
		output_current2 = pidProcess(&s2_speed, vw2, m2006.m2.speed);
		//三号轮速度PID控制   
		output_current3 = pidProcess(&s3_speed, vw3, m2006.m3.speed);
		
    // printf("vw: %lf %lf %lf\r\n",vw1, vw2, vw3); 
    //	printf("speed: %d %d %d\r\n",m2006.m1.speed, m2006.m2.speed, m2006.m3.speed);
  	//	printf("output: %lf %lf %lf\r\n",output_current1, output_current2, output_current3);
	
			//四号轮速度PID控制
		//output_current4 = pidProcess(&s4_speed, vw4, m2006.m4.speed);
		
		//此处再在原来基础上加上对于角度的pid控制，类似于追球的
		
		
		//前四个ID的2006电机
		m2006ctrl.pack = M2006_PACK_1_4;
		//输出给一号轮电流
		m2006ctrl.setcur[0] = (int16_t)output_current1;
		//输出给二号轮电流
		m2006ctrl.setcur[1] = (int16_t)output_current2;
		//输出给三号轮电流
		m2006ctrl.setcur[2] = (int16_t)output_current3;
		//输出给四号轮电流
		m2006ctrl.setcur[3] = (int16_t)output_current4;
		//通过CAN总线发送
		m2006SetCurrent(&m2006ctrl,&hcan2);
		
		//向A型主控返回电机转子的速度
//			msg[0]=(m2006.m1.speed&0xFF00)>>8;
//			msg[1]=m2006.m1.speed&0x00FF;
//			msg[2]=(m2006.m3.speed&0xFF00)>>8;
//			msg[3]=m2006.m3.speed&0x00FF;
//			msg[4]=(m2006.m2.speed&0xFF00)>>8;
//			msg[5]=m2006.m2.speed&0x00FF;
//			msg[6]=(m2006.m4.speed&0xFF00)>>8;
//			msg[7]=m2006.m4.speed&0x00FF;
//			Can1SendMsg_TEST(msg,0x403);
			
//计数圈数
//id为1的轮子
			msg[0]=(rcount[0]&0xFF00)>>8;
			msg[1]=rcount[0]&0x00FF;
//id=2的轮子的转动圈数
			msg[2]=(rcount[1]&0xFF00)>>8;
			msg[3]=rcount[1]&0x00FF;
//id=3的轮子的转动圈数
			msg[4]=(rcount[2]&0xFF00)>>8;
			msg[5]=rcount[2]&0x00FF;
//id=4的轮子的转动圈数
			msg[6]=(rcount[3]&0xFF00)>>8;
			msg[7]=rcount[3]&0x00FF;
			//	Can1SendMsg_TEST(msg,0x403); 向上位机返回数据  通过can1或者usart2
		//延时10ms
		HAL_Delay(5);
	//}
}

//PID初始化
void Pid_Init(void)
{
//一号轮子的PID参数
	//PID使能
	s1_speed.enable 		        = 1;	
	//PID使能积分限幅
	s1_speed.enable_lim_sum_error  = 1;
	//PID使能输出限幅
	s1_speed.enable_lim_ouput	    = 1;
	//PID比例参数
	s1_speed.kp                    = 240;//	530       //  3号轮180较合适
	//PID积分参数
	s1_speed.ki                    = 25; 
	//PID微分参数
	s1_speed.kd                    = 20;
	//PID积分限幅
	s1_speed.lim_sum_error         = 2500;
	//PID输出限幅
	s1_speed.lim_output            = 10000;
	//均值滤波器
  s1_speed.hfilter1.a            = 0;
	
//二号轮子的PID参数
  //PID使能
  s2_speed.enable 		        = 1;	
	//PID使能积分限幅
	s2_speed.enable_lim_sum_error  = 1;
	//PID使能输出限幅
	s2_speed.enable_lim_ouput	    = 1;
	//PID比例参数
	s2_speed.kp                    = 195;//	530       //  3号轮180较合适                 因为场地的硬度不同 就会出现不同的pid  越软越容易发散  
	//PID积分参数
	s2_speed.ki                    = 25; 
	//PID微分参数
	s2_speed.kd                    = 20;
	//PID积分限幅
	s2_speed.lim_sum_error         = 2500;
	//PID输出限幅
	s2_speed.lim_output            = 10000;
	//均值滤波器 
  s2_speed.hfilter1.a            = 0; 
	
//三号轮子的PID参数
	//PID使能
  s3_speed.enable 		        = 1;	
	//PID使能积分限幅
	s3_speed.enable_lim_sum_error  = 1;
	//PID使能输出限幅
	s3_speed.enable_lim_ouput	    = 1;
	//PID比例参数 
	s3_speed.kp                    = 240;//	530       //  3号轮180较合适
	//PID积分参数
	s3_speed.ki                    = 25; 
	//PID微分参数
	s3_speed.kd                    = 20;
	//PID积分限幅
	s3_speed.lim_sum_error         = 2500;
	//PID输出限幅
	s3_speed.lim_output            = 10000;
	//均值滤波器

	//四号轮子的PID参数
	//s4_speed = pid_speed;
	
	//imu的PID参数
	//PID使能
	imu_data.enable 		        = 1;	
	//PID使能积分限幅
	imu_data.enable_lim_sum_error  = 1;
	//PID使能输出限幅
	imu_data.enable_lim_ouput	    = 1;
	//PID比例参数
	imu_data.kp                    = 0.5;//	530       //  3号轮180较合适
	//PID积分参数
	imu_data.ki                    = 0; 
	//PID微分参数
	imu_data.kd                    = 0;
	//PID积分限幅
	imu_data.lim_sum_error         = 500;
	//PID输出限幅
	imu_data.lim_output            = 50;
}


void ChassisDatasHandle(CAR *CAR_Initure)//底盘can接收数据处理函数
{
	uint8_t CanRxDatas[8];//接收数据
	uint16_t Can1RecDataID;//接收数据的id
	Can1RecDataID=Can1RecDatas(CanRxDatas,CAN_FIFO0);//获取id
	if(Can1RecDataID==0x400)//
	{
		Car.vx=((CanRxDatas[0]<<8)|CanRxDatas[1]);
		Car.vy=((CanRxDatas[2]<<8)|CanRxDatas[3]);
		Car.w=(int)(CanRxDatas[4]<<8)|CanRxDatas[5];
	
	}
//	else//
//	{
//		OpenmvEnableFlag=CanRxDatas[0];
//	}
		
}

//CAN接受中断
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
	if(hcan->Instance==CAN2){
	  if (HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0) != HAL_OK)
    {  
      __HAL_CAN_ENABLE_IT(&hcan2, CAN_IT_FMP0);// set interrupt flag for RX FIFO0 if CAN locked
    }
	  m2006FeedBack(&hcan2,&m2006);//反馈函数 
  }
	else if(hcan->Instance==CAN1){
		 if (HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0) != HAL_OK)
    {  
      __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_FMP0);// set interrupt flag for RX FIFO0 if CAN locked
    }
		if(hcan->pRxMsg->StdId==0x200||hcan->pRxMsg->StdId==0x401||hcan->pRxMsg->StdId==0x402)
			;
		else if(hcan->pRxMsg->StdId==0x403)
			;
		else if(hcan->pRxMsg->StdId==0x404)
		{
			if(hcan->pRxMsg->Data[0]==1)
					m2006ResetRoundcnt(&m2006);
		}
		else
		  ChassisDatasHandle(&Car);//处理主控发给地盘的数据
//				 if (HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0) != HAL_OK)
//    {  
//      __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_FMP0);// set interrupt flag for RX FIFO0 if CAN locked
//    }
	}
		
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	if (HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0) != HAL_OK)
  {  
    __HAL_CAN_ENABLE_IT(&hcan2, CAN_IT_FMP0);// set interrupt flag for RX FIFO0 if CAN locked
  }
	if (HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0) != HAL_OK)
  {  
    __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_FMP0);// set interrupt flag for RX FIFO0 if CAN locked
  }
}

