#ifndef _MODULE_CHASSIS_H
#define _MODULE_CHASSIS_H

#include "device_m2006.h"
#include "math.h"
#include "pid.h"
#include "oled.h"

//底盘句柄
typedef struct
{
		//状态共用体
    union
    {   
        uint8_t value;
        struct      
        {
						//力矩使能
            uint8_t entorque    :   1   ;  
						//平动速度数据有效
            uint8_t vldspeed    :   1   ;
						//平动方向数据有效
            uint8_t vlddir      :   1   ;
						//目标角度数据有效					
            uint8_t vldangle    :   1   ; 
						 //角速度数据有效
            uint8_t vldangv     :   1   ;  
        }CMCTROLTATE;
    }cm_ctrolstate;
		//平动速度
    uint16_t speed;
		//平动方向
    uint16_t dir;
		 //转动角速度	
    int16_t angv;                   
} CHASSIS_HANDLE,*P_CHASSIS_HANDLE;

//小车运动结构体
typedef struct
{
	//小车x方向速度
	int16_t vx;
	//小车y方向速度
	int16_t vy;
	//小车转动角速度
	int16_t w;
	//小车转动方向
	uint8_t angv_dir;
}CAR;

void Chassis(void);


#endif


