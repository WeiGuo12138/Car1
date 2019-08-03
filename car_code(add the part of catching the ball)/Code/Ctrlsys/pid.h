
#ifndef __PID_H
#define __PID_H

#include "ctrlsys.h"


typedef struct
{
	uint8_t enable 					: 1;	//输出使能
	uint8_t enable_lim_sum_error	: 1;	//积分限幅使能
	uint8_t enable_lim_ouput		: 1;	//输出限幅使能
	uint8_t count					: 4;	
	uint8_t 						: 1;	

	double kp;  			//比例参数
	double ki;  			//积分参数
	double kd;  			//微分参数

	double ctrlcycle;  		//以ms为单位的控制周期
	
	double lim_sum_error;	//误差积分限幅	
	double lim_output;		//输出限幅

	double sum_error;		//误差积分
	double last_error;		//上一次的误差

	FILTER1_HANDLE hfilter1;//惯性滤波器
} PID_HANDLE, *P_PID_HANDLE;

void pidReset(P_PID_HANDLE phdl);
double pidProcess(P_PID_HANDLE phdl, double input, double measure);
double imu_pid(P_PID_HANDLE phdl, double input);



#endif
