#include "module_arm.h"

//机械臂抓取位置，默认从前到后依次抓取，1代表从前到后第一个物块
uint8_t Location[3]={1,2,3};
//物块摆放颜色位置，默认按抓取顺序依次为红，绿，蓝
uint8_t ColorLocation[3]={1,2,3};
uint8_t PawFlag;//机械爪的命令标志位，用于判断机械爪是否张开收紧
uint8_t ArmFinishFlag=0;//机械臂动作完成标志位，用于判断动作是否做完

//电流
double ArmOutputCurrent = 0;

//爪子电机速度环PID参数
PID_HANDLE PawPidSpeed = {0};
//爪子电机角度环PID参数
PID_HANDLE PawPidAngle = {0};
//舵机角度环PID参数
PID_HANDLE ServoPidAngle = {0};

//舵机测试
SERVO_HandleTypeDef SERVO;
//一号舵机
SERVO_HandleTypeDef SERVO1;
//二号舵机
SERVO_HandleTypeDef SERVO2;
//五号舵机，底座
SERVO_HandleTypeDef SERVO5;

DEF_ARM arm1;//机械臂结构体

uint16_t angle_test=500,time_test=1000;
uint8_t ID_test=3;

//所有舵机角度回读
void ArmGetPos(void)
{
  SERVO_POS_READ(&SERVO5);//读取5号舵机的实时位置
	SERVO_POS_READ(&SERVO2);//读取2号舵机的实时位置
  SERVO_POS_READ(&SERVO1);//读取1号舵机的实时位置	
}

//舵机终点判断函数,判断舵机是否转到终点，error为误差值
uint8_t ServoEndJudging(SERVO_HandleTypeDef *Servo,uint8_t error)//舵机终点判断函数,判断舵机是否转到终点
{
	if(Servo->ActAngle-Servo->TagAngle>-error&&Servo->ActAngle-Servo->TagAngle<error)
		return 1;//到达终点，则返回1
	else
		return 0;//否则，返回0
}

/*px机械臂腕部目标点x坐标，目
标点y坐标，theta3是2号舵机与水平面的夹角
*/
//int16_t Angle1=500,Angle2=500,Angle3,Speed5;//全局变量，测试用
void ArmDriver(int16_t Angle1,int16_t Angle2,int16_t Angle5,uint16_t TagAngleDelayTime)
{
	if(Angle1>S1_MAXANGLE) Angle1=S1_MAXANGLE;
	if(Angle1<S1_MINANGLE) Angle1=S1_MINANGLE;
	if(Angle2>S2_MAXANGLE) Angle2=S2_MAXANGLE;
	if(Angle2<S2_MINANGLE) Angle2=S2_MINANGLE;
	SERVO5.TagAngle=Angle5;
  SERVO5.Speed=pidProcess(&ServoPidAngle,SERVO5.TagAngle,SERVO5.ActAngle);//pid调节
	if(SERVO5.Speed>300) SERVO5.Speed=300;
	if(SERVO5.Speed<-300) SERVO5.Speed=-300;
	SERVO2.TagAngle=Angle2;
	SERVO1.TagAngle=Angle1;
	SERVO1.TagAngleDelayTime=TagAngleDelayTime;
	SERVO2.TagAngleDelayTime=TagAngleDelayTime;
}

void ArmPidInit(void)
{
	//PID使能
	PawPidSpeed.enable 		        = 1;	
	PawPidAngle.enable            = 1;
	ServoPidAngle.enable            = 1;
	//PID使能积分限幅
	PawPidSpeed.enable_lim_sum_error  = 1;
	PawPidAngle.enable_lim_sum_error  = 1;
	ServoPidAngle.enable_lim_sum_error  = 1;
	//PID使能输出限幅
	PawPidSpeed.enable_lim_ouput	    = 1;
	PawPidAngle.enable_lim_ouput	    = 1;
	ServoPidAngle.enable_lim_ouput      = 1;
	//PID比例参数
	PawPidSpeed.kp                    = 3.6;
	PawPidAngle.kp                    = 0.1;
	ServoPidAngle.kp                    = 2;
	//PID积分参数
	PawPidSpeed.ki                    = 0.5; 
	PawPidAngle.ki                    = 0; 
	ServoPidAngle.ki                    = 0.05;
	//PID微分参数
	PawPidSpeed.kd                    = 2;
	PawPidAngle.kd                    = 0;
	ServoPidAngle.kd                    = 0;
	//PID积分限幅
	PawPidSpeed.lim_sum_error         = 1500;
	PawPidAngle.lim_sum_error         = 1500;
	ServoPidAngle.lim_sum_error         = 1500;
	//PID输出限幅
	PawPidSpeed.lim_output            = 15000;
	PawPidAngle.lim_output            = 15000;
	ServoPidAngle.lim_output            = 15000;
	//均值滤波器
  PawPidSpeed.hfilter1.a            = 0;
	PawPidAngle.hfilter1.a            = 0;
	ServoPidAngle.hfilter1.a            = 0;
}

//机械臂初始化
void ArmInit(void)
{
	//pid初始化
	ArmPidInit();
	//定义各个舵机的ID和模式 ServoMode为舵机模式  MotorMode为电机模式
  SERVO1.ID=0x01;
	SERVO1.mode=ServoMode;
	SERVO2.ID=0x02;
	SERVO2.mode=ServoMode;
	SERVO5.ID=0x05;
	SERVO5.mode=MotorMode;
	arm1.ArmState=ARM_START;//机械臂状态初始化
}

void ArmPutDownLater(uint8_t ColorLocation)//准备动作1
{
	switch(ColorLocation)
	{
		case 3:
			ArmDriver(650,900,PutDown_RIGHT,1000);
		  break;
		case 2:
			ArmDriver(650,900,PutDown_CENT,1000);
		  break;
		default:
			ArmDriver(650,900,PutDown_LEFT,1000);
		  break;
	}
}

void ArmPutUpReady(uint8_t Location)//准备动作2
{
	switch(Location)
	{
		case 1:
			ArmDriver(650,900,PutUp_LEFT,1000);
		  break;
		case 2:
			ArmDriver(650,900,PutUp_CENT,1000);
		  break;
		default:
			ArmDriver(650,900,PutUp_RIGHT,1000);
		  break;
	}
}

void ArmPutUpBefore(uint8_t Location)//抓取准备动作3
{
	switch(Location)
	{
		case 1:
			ArmDriver(200,900,SERVO5.TagAngle,1000);
		  break;
		case 2:
			ArmDriver(200,900,SERVO5.TagAngle,1000);
		  break;
		default:
			ArmDriver(200,900,SERVO5.TagAngle,1000);
		  break;
	}
}

void ArmPutDownReady(uint8_t ColorLocation)//放下准备动作1
{
	switch(ColorLocation)
	{
		case 3:
			ArmDriver(650,900,PutDown_RIGHT,1000);
		  break;
		case 2:
			ArmDriver(650,900,PutDown_CENT,1000);
		  break;
		default:
			ArmDriver(650,900,PutDown_LEFT,1000);
		  break;
	}
}

void ArmPutUpLater(uint8_t Location)//放下准备动作2
{
	switch(Location)
	{
		case 1:
			ArmDriver(650,900,PutUp_LEFT,1000);
		  break;
		case 2:
			ArmDriver(650,900,PutUp_CENT,1000);
		  break;
		default:
			ArmDriver(650,900,PutUp_RIGHT,1000);
		  break;
	}
}

void ArmPutDownBefore(uint8_t ColorLocation)//放下准备动作3
{
	switch(ColorLocation)
	{
		case 3:
			ArmDriver(200,900,PutDown_RIGHT,1000);
		  break;
		case 2:
			ArmDriver(200,900,PutDown_CENT,1000);
		  break;
		default:		
			ArmDriver(200,900,PutDown_LEFT,1000);
		  break;
	}
}
//抓取变小，松开变大
void ArmPawCtrl(uint8_t PawFlag)
{
	if(PawFlag==0)//抓取
	{
    ArmPawDriver(-4);
	}
	else//松开
	{
      ArmPawDriver(0); 
	}
}
void OpenmvDataHandle(void)//openmv数据处理，判断机械臂抓取顺序
{
	uint8_t ColorCnt;//遍历颜色数组的计数变量
	uint8_t QRCnt;//遍历二维码数组的计数变量
	for(ColorCnt=0;ColorCnt<3;ColorCnt++)
	{
		for(QRCnt=0;QRCnt<3;QRCnt++)
		{
			if(openmv_1.color.color_order[ColorCnt]==openmv_1.qr.Qrcode[QRCnt]){
				Location[QRCnt]=ColorCnt+1;//确定抓取顺序
				ColorLocation[QRCnt]=openmv_1.qr.Qrcode[QRCnt];//确定放下顺序
			}
		}
	}
}

uint8_t ArmFineTuning(uint8_t Object)//机械臂微调函数，传入参数是机械臂微调时的参考对象
{
	if(Object==Block)//如果参考对象是物块的位置
	{
		SERVO5.Speed=pidProcess(&ServoPidAngle,TAGLOCATION,openmv_1.ObjLocation.x);
		if(TAGLOCATION-openmv_1.ObjLocation.x<10)
			return Center;
	}
	return 0;
}

void ArmPawDriver(double GoalCycleNum)//形参：目标圈数
{
	double GoalSpeed,GoalAngle;//目标速度,目标角度
	GoalAngle=GoalCycleNum*8192;
	//位置PID控制
	GoalSpeed=pidProcess(&PawPidAngle,GoalAngle,m2006.m1.sum_angle);
	//速度PID控制
	ArmOutputCurrent = pidProcess(&PawPidSpeed, GoalSpeed, m2006.m1.speed);
	//前四个ID的2006电机
	m2006ctrl.pack = M2006_PACK_1_4;
	//输出给一号轮电流
	m2006ctrl.setcur[0] = (int16_t)ArmOutputCurrent;
	//通过CAN总线发送
	m2006SetCurrent(&m2006ctrl,&hcan1);
}

//void ArmMsgUpdate(void)
//{
//	ArmGetPos();//舵机角度回读
//	switch(arm1.ArmState)
//	{
//		case ARM_START://调整openmv镜头至扫颜色模式
//			OpenmvCloudTerraceDriver(500,500);//初始化云台，进入扫颜色模式
//		  if(track_1.state==FORWARD_LINE4&&!ArmFinishFlag)
//				arm1.ArmState=OP_START;//跳转机械臂系统的状态开始扫码
//		  break;
//		case OP_START://调整openmv镜头开始扫码
//			OpenmvCloudTerraceDriver(500,500);//扫码模式
//		  if(!ArmFinishFlag) arm1.ArmState=OP_FINETUNING;//跳转机械臂系统的状态进行扫码微调
//		  break;
//		case OP_FINETUNING://扫码微调
//			break;
//		case CARRY1_1://第一次搬运，动作1
//			//OpenmvDataHandle();
//			OpenmvCloudTerraceDriver(500,500);//隐藏摄像头云台，防止干涉
//			ArmPutUpReady(Location[0]);//准备抓第一个物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY1_2;//跳转状态
//      break;
//		case CARRY1_2:
//			ArmFineTuning(Block);//微调
//			if(OpenmvObjectDetection(Block)==Center)
//				arm1.ArmState=CARRY1_4;//准备合上爪子
//		  break;
//		case CARRY1_3://开始抓第一个物块
//			ArmPutUpBefore(Location[0]);//准备抓第一个物块
//			if(!ArmFinishFlag) arm1.ArmState=CARRY1_3;//跳转状态
//		  break;
//		case CARRY1_4:
//			PawFlag=1;//抓物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY1_5;//跳转状态
//		  break;
//		case CARRY1_5:
//			ArmPutUpLater(Location[0]);//抓完第一个物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY1_6;//跳转状态
//		  break;
//		case CARRY1_6:
//			ArmPutDownReady(ColorLocation[0]);//放下准备动作2
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY1_7;//跳转状态
//			break;
//		case CARRY1_7://放下第一个物块
//			ArmPutDownBefore(ColorLocation[0]);//准备放第一个物块
//      if(!ArmFinishFlag) arm1.ArmState=CARRY1_8;//跳转状态
//		  break;
//		case CARRY1_8:
//		  PawFlag=0;//放物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY1_9;//跳转状态
//		  break;
//		case CARRY1_9:
//			ArmPutDownLater(ColorLocation[0]);//放完第一个物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY2_1;//跳转状态
//		  break;
//		case CARRY2_1://抓取准备
//			ArmPutUpReady(Location[1]);//准备抓第二个物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY2_2;//跳转状态
//			break;
//		case CARRY2_2://开始抓第二个物块
//			ArmPutUpBefore(Location[1]);//准备抓第二个物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY2_3;//跳转状态
//		  break;
//		case CARRY2_3:
//			ArmFineTuning(Block);//微调
//			if(OpenmvObjectDetection(Block)==Center)
//				 arm1.ArmState= CARRY2_4;
//			break;
//		case CARRY2_4:
//			PawFlag=1;//抓物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY2_5;//跳转状态
//		case CARRY2_5:
//			ArmPutUpLater(Location[1]);//抓完第二个物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY2_6;//跳转状态
//		  break;
//		case CARRY2_6:
//			ArmPutDownReady(ColorLocation[1]);//放下准备动作2
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY2_7;//跳转状态
//			break;
//		case CARRY2_7://放下第二个物块
//			ArmPutDownBefore(ColorLocation[1]);//准备放第二个物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY2_8;//跳转状态
//		  break;
//		case CARRY2_8:
//			PawFlag=0;//放物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY2_9;//跳转状态
//		  break;
//		case CARRY2_9:
//			ArmPutDownLater(ColorLocation[1]);//放完第二个物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY3_1;//跳转状态
//		  break;
//		case CARRY3_1://抓取准备
//			ArmPutUpReady(Location[2]);//准备抓第三个物块
//			if(!ArmFinishFlag) arm1.ArmState=CARRY3_2;//跳转状态
//			break;
//		case CARRY3_2://开始抓第三个物块
//			ArmPutUpBefore(Location[2]);//准备抓第三个物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY3_3;//跳转状态
//			break;
//		case CARRY3_3:
//			ArmFineTuning(Block);//微调
//			if(OpenmvObjectDetection(Block)==Center)
//				arm1.ArmState=CARRY3_4;
//			break;
//		case CARRY3_4:
//			PawFlag=1;//抓物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY3_5;//跳转状态
//		case CARRY3_5:
//			ArmPutUpLater(Location[2]);//抓完第三个物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY3_6;//跳转状态
//		  break;
//		case CARRY3_6:
//			ArmPutDownReady(ColorLocation[2]);//放下准备动作2
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY3_7;//跳转状态
//			break;
//		case CARRY3_7://放下第三个物块
//			ArmPutDownBefore(ColorLocation[2]);//准备放第三个物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY3_8;//跳转状态
//		  break;
//		case CARRY3_8:
//			PawFlag=0;//放物块
//		  if(!ArmFinishFlag) arm1.ArmState=CARRY3_9;//跳转状态
//		  break;
//		case CARRY3_9:
//			ArmPutDownLater(ColorLocation[2]);//放完第三个物块
//		  if(!ArmFinishFlag) arm1.ArmState=ARM_STOP;//跳转状态
//		  break;
//		default:
//			break;
//}
//			}

void ArmBodyCtrl(void)
{
	ServoDriver(&SERVO1);//1号舵机动作
  ServoDriver(&SERVO2);//2号舵机动作
}	
void ArmBaseCtrl(void)
{
	ServoDriver(&SERVO5);//5号舵机动作
}

void PawRemoteTest(int16_t ch)
{
	if(ch>500)
		PawFlag=1;
	else
		PawFlag=0;
}

void Arm(void)
{
//	ArmMsgUpdate();
	//下列代码是用于测试机械臂动作组的模型，用完删
	ArmGetPos();
	switch(arm1.ArmState)
	{
		case ARM_START:
			ArmDriver(650,900,PutUp_CENT,1000);
		  if(ServoEndJudging(&SERVO5,10)&&ServoEndJudging(&SERVO1,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY1_1;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
			break;
		case CARRY1_1://动作1
			ArmPutUpReady(Location[0]);//准备抓第一个物块
			if(ServoEndJudging(&SERVO5,10)&&ServoEndJudging(&SERVO1,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY1_3;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
		  break;
		case CARRY1_2://动作2,机械臂微调
			if(ArmFineTuning(Block)==Center)
				arm1.ArmState=CARRY1_3;//准备合上爪子
		  break;
		case CARRY1_3://动作3
			ArmPutUpBefore(Location[0]);
		  if(ServoEndJudging(&SERVO5,10)&&ServoEndJudging(&SERVO1,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY1_4;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
			break;
		case CARRY1_4:
			PawFlag=1;//抓物块
		  arm1.ArmState=CARRY1_5;//跳转状态
		  break;
		case CARRY1_5:
			ArmPutUpLater(Location[0]);//抓完第一个物块
		  if(ServoEndJudging(&SERVO1,10)&&ServoEndJudging(&SERVO5,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY1_6;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
		  break;
		case CARRY1_6:
			ArmPutDownReady(ColorLocation[0]);//放下准备动作2
		  if(ServoEndJudging(&SERVO5,10)&&ServoEndJudging(&SERVO1,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY1_7;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
			break;
		case CARRY1_7://放下第一个物块
			ArmPutDownBefore(ColorLocation[0]);//准备放第一个物块
      if(ServoEndJudging(&SERVO1,10)&&ServoEndJudging(&SERVO5,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY1_8;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
		  break;
		case CARRY1_8:
		  PawFlag=0;//放物块
		  arm1.ArmState=CARRY1_9;//跳转状态
		  break;
		case CARRY1_9:
			ArmPutDownLater(ColorLocation[0]);//放完第一个物块
		  if(ServoEndJudging(&SERVO1,10)&&ServoEndJudging(&SERVO5,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY2_1;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
		  break;
			case CARRY2_1://动作1
			ArmPutUpReady(Location[1]);//准备抓第一个物块
			if(ServoEndJudging(&SERVO5,10)&&ServoEndJudging(&SERVO1,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY2_3;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
		  break;
		case CARRY2_2://动作2,机械臂微调
			if(ArmFineTuning(Block)==Center)
				arm1.ArmState=CARRY2_3;//准备合上爪子
		  break;
		case CARRY2_3://动作3
			ArmPutUpBefore(Location[1]);
		  if(ServoEndJudging(&SERVO1,10)&&ServoEndJudging(&SERVO5,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY2_4;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
			break;
		case CARRY2_4:
			PawFlag=1;//抓物块
		  arm1.ArmState=CARRY2_5;//跳转状态
		  break;
		case CARRY2_5:
			ArmPutUpLater(Location[1]);//抓完第一个物块
		  if(ServoEndJudging(&SERVO1,10)&&ServoEndJudging(&SERVO5,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY2_6;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
		  break;
		case CARRY2_6:
			ArmPutDownReady(ColorLocation[1]);//放下准备动作2
		  if(ServoEndJudging(&SERVO5,10)&&ServoEndJudging(&SERVO1,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY2_7;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
			break;
		case CARRY2_7://放下第一个物块
			ArmPutDownBefore(ColorLocation[1]);//准备放第一个物块
      if(ServoEndJudging(&SERVO1,10)&&ServoEndJudging(&SERVO5,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY2_8;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
		  break;
		case CARRY2_8:
		  PawFlag=0;//放物块
		  arm1.ArmState=CARRY2_9;//跳转状态
		  break;
		case CARRY2_9:
			ArmPutDownLater(ColorLocation[1]);//放完第一个物块
		  if(ServoEndJudging(&SERVO1,10)&&ServoEndJudging(&SERVO5,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY3_1;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
		  break;
			case CARRY3_1://动作1
			ArmPutUpReady(Location[2]);//准备抓第一个物块
			if(ServoEndJudging(&SERVO1,10)&&ServoEndJudging(&SERVO5,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY3_3;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
		  break;
		case CARRY3_2://动作2,机械臂微调
			if(ArmFineTuning(Block)==Center)
				arm1.ArmState=CARRY3_3;//准备合上爪子
		  break;
		case CARRY3_3://动作3
			ArmPutUpBefore(Location[2]);
		  if(ServoEndJudging(&SERVO1,10)&&ServoEndJudging(&SERVO5,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY3_4;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
			break;
		case CARRY3_4:
			PawFlag=1;//抓物块
		  arm1.ArmState=CARRY3_5;//跳转状态
		  break;
		case CARRY3_5:
			ArmPutUpLater(Location[2]);//抓完第一个物块
		  if(ServoEndJudging(&SERVO1,10)&&ServoEndJudging(&SERVO5,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY3_6;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
		  break;
		case CARRY3_6:
			ArmPutDownReady(ColorLocation[2]);//放下准备动作2
		  if(ServoEndJudging(&SERVO1,10)&&ServoEndJudging(&SERVO5,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY3_7;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
			break;
		case CARRY3_7://放下第一个物块
			ArmPutDownBefore(ColorLocation[2]);//准备放第一个物块
      if(ServoEndJudging(&SERVO1,10)&&ServoEndJudging(&SERVO5,10))//如果到达目标位置
			{
				arm1.ArmState=CARRY3_8;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
		  break;
		case CARRY3_8:
		  PawFlag=0;//放物块
		  arm1.ArmState=CARRY3_9;//跳转状态
		  break;
		case CARRY3_9:
			ArmPutDownLater(ColorLocation[2]);//放完第一个物块
		  if(ServoEndJudging(&SERVO1,10)&&ServoEndJudging(&SERVO5,10))//如果到达目标位置
			{
				arm1.ArmState=ARM_STOP;//跳转机械臂系统的状态
		    ArmFinishFlag=0;
			}
		  break;
		default:
			break;
	}
	ArmBaseCtrl();//底座控制
	ArmPawCtrl(PawFlag);
//	HAL_Delay(1000);
	if(ArmFinishFlag==0)//如果某一组动作未完成
	{
		ArmBodyCtrl();
		ArmFinishFlag=1;
	}
//	ArmStateRemoteTest();
	if(TaskCnt%100==0)//每1秒对ArmFinishFlag清零一次
	{
		ArmFinishFlag=0;
//	  TELL_OP_MV();
	}
	
//	HAL_Delay(1000);
	  
//	MotorRemoteTest(&SERVO5,rc.ch3);
//	MotorRemoteTest(&SERVO2,rc.ch4);
//	MotorRemoteTest(&SERVO1,rc.ch2);
//	MotorRemoteTest(&SERVO3,rc.ch1);
//	PawRemoteTest(rc.ch1);
}



