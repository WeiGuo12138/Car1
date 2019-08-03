#ifndef __MOUDLE_ARM_H
#define __MOUDLE_ARM_H

#include "device_steer.h"
#include "math.h"
#include "bsp_tim.h"
#include "device_m2006.h"
#include "pid.h"
#include "openmv.h"

#define ArmL1 10
#define ArmL2 10
#define pi 3.14
//定义舵机可转最大物理角度，防止损坏机械臂
#define S1_MAXANGLE 650
#define S1_MINANGLE 200
#define S2_MAXANGLE 900
#define S2_MINANGLE 800

#define PutUp_LEFT 800
#define PutUp_CENT 900
#define PutUp_RIGHT 1000
#define PutDown_LEFT 300
#define PutDown_CENT 100
#define PutDown_RIGHT -100

#define LeftSide 1  //偏左 宏定义 用于检测目标物块相对于爪部的位置
#define RightSide 2  //偏右
#define Center  3 //中心
#define UpSide 4  //偏上
#define DownSide 5 //偏下

#define QR 1   //宏定义，确定摄像机检测的目标类型
#define Block 2

#define TAGLOCATION 100

typedef struct//物体相对于机械臂的坐标值
{
	uint8_t x,y;
}OBJ_LOCATION;

typedef enum//当前机械臂状态枚举
{
	ARM_START,//出车，机械臂，摄像头云台初始化
	OP_START,//开始扫码
	OP_FINETUNING,//扫码微调
	CARRY1_1,//第一次搬运，动作一
	CARRY1_2,//第一次搬运，动作二
	CARRY1_3,//第一次搬运，动作三
	CARRY1_4,//第一次搬运，动作四
	CARRY1_5,//第一次搬运，动作五
	CARRY1_6,//第一次搬运，动作六
	CARRY1_7,//第一次搬运，动作七
	CARRY1_8,//第一次搬运，动作八
	CARRY1_9,//第一次搬运，动作九
	CARRY2_1,//第二次搬运，动作一
	CARRY2_2,//第二次搬运，动作二
	CARRY2_3,//第二次搬运，动作三
	CARRY2_4,//第二次搬运，动作四
	CARRY2_5,//第二次搬运，动作五
	CARRY2_6,//第二次搬运，动作六
	CARRY2_7,//第二次搬运，动作七
	CARRY2_8,//第二次搬运，动作八
	CARRY2_9,//第二次搬运，动作九
	CARRY3_1,//第三次搬运，动作一
	CARRY3_2,//第三次搬运，动作二
	CARRY3_3,//第三次搬运，动作三
	CARRY3_4,//第三次搬运，动作四
	CARRY3_5,//第三次搬运，动作五
	CARRY3_6,//第三次搬运，动作六
	CARRY3_7,//第三次搬运，动作七
	CARRY3_8,//第三次搬运，动作八
	CARRY3_9,//第三次搬运，动作九
	ARM_STOP//结束
}DEF_ARM_STATE;

typedef struct
{
	OBJ_LOCATION ObjectLocation;//目标相对于摄像头的坐标值
	DEF_ARM_STATE ArmState;//当前机械臂状态枚举
}DEF_ARM;//机械臂结构体

extern uint8_t CmdFlag;
extern double v;
extern M2006_FDBK m2006;
extern DEF_ARM arm1;
extern M2006_CTRL  m2006ctrl;

void ArmDriver(int16_t Angle1,int16_t Angle2,int16_t Angle5,uint16_t TagAngleDelayTime);
uint8_t ArmBaseDriver(int16_t Speed,int16_t Angle);
void ArmTest1(void);
void ArmInit(void);
void Arm(void);
void ArmPawDriver(double v);
void ArmPawCtrl(uint8_t PawFlag);
void ServoFeedBack(uint8_t *RxDatas);
void ServoDataHandle(uint8_t Data,uint8_t *RxDatas);
void OpenmvFeedBack(OBJ_LOCATION *ObjectLocation);
 
#endif
