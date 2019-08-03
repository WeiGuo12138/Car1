#include "wl_laserdis.h"
#include "delay.h"

VL53L0X_Dev_t                       wl_laserdis_dev;            //设备结构体变量，保存设备信息
VL53L0X_RangingMeasurementData_t    wl_laserdis_data;           //测量数据结构体，保存测量结果
unsigned int			            wl_laserdis_disance;		//保存测量结果
/***************************************************************************************/

/*********************************************************************
 *  函数名称：int8_t  Wl_laserdis_start_single_test(VL53L0X_Dev_t *pdev, VL53L0X_RangingMeasurementData_t *pdata)
 *  函数功能：执行单次测量
 *  形    参：*pMyDevice：设备结构体，保存了设备的通信信息
 *			  *pdata：测量数据结构体
 *  输    出：初始化状态：0：成功	其它：失败
 *  备    注：无
 ********************************************************************/
int8_t  Wl_laserdis_start_single_test(VL53L0X_Dev_t *pdev, VL53L0X_RangingMeasurementData_t *pdata)
{
    int8_t Status = 0;
	
    Status = VL53L0X_PerformSingleRangingMeasurement(pdev, pdata);   //VL53L0X执行单一测量
		printf("%d \r\n",wl_laserdis_disance);
	if(Status == 0)
	{
		if((pdata->RangeMilliMeter > 20) && (pdata->RangeMilliMeter < 8100))	//数据正确
		{		
			wl_laserdis_disance = pdata->RangeMilliMeter;
			printf("%d \r\n",wl_laserdis_disance);
		}
	}
		
    return Status;
}

/*********************************************************************
 *  函数名称：int8_t Wl_laserdis_measure_init(VL53L0X_Dev_t *pMyDevice, uint8_t ranging_mode, uint8_t accuracy_mode)
 *  函数功能：设置测量模式
 *  形    参：*pMyDevice：设备结构体，保存了设备的通信信息
 *			  ranging_mode：测量模式	0：单次测量模式		1：连续测量模式
 *			  accuracy_mode：精度模式	0：默认		1：高精度	2：长距离	3：高速
 *  输    出：初始化状态：0：成功	其它：失败
 *  备    注：无
 ********************************************************************/
int8_t Wl_laserdis_measure_init(VL53L0X_Dev_t *pMyDevice, uint8_t ranging_mode, uint8_t accuracy_mode)
{
    int8_t Status = 0;

	if(Status == 0)		//设置测量模式
	{
		if(ranging_mode)					//连续测量模式
			Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING); 
		else								//单次测量模式
			Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_SINGLE_RANGING); 
	}

    if(Status == 0)// 启用/禁用特定限制检查
        Status = VL53L0X_SetLimitCheckEnable(pMyDevice, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);
    if(Status == 0)
        Status = VL53L0X_SetLimitCheckEnable(pMyDevice, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);

	//不同的精度需要设置的参数不一致
	if(accuracy_mode == 0)		//默认模式
	{
		if (Status == 0)			//设定信号速率范围范围
			Status = VL53L0X_SetLimitCheckValue(pMyDevice, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.25*65536));
		if (Status == 0)			//设定SIGMA范围
			Status = VL53L0X_SetLimitCheckValue(pMyDevice, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(18*65536));			
		if (Status == 0)			//设定完整测距最长时间
			Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice, 33000);
		if (Status == 0)			//设定VCSEL脉冲周期
			Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 14);
		if (Status == 0) 			//设定VCSEL脉冲周期范围
			Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 10);
	}
	else if(accuracy_mode == 1)		//高精度测量模式
	{
		if (Status == 0)			//设定信号速率范围范围			
			Status = VL53L0X_SetLimitCheckValue(pMyDevice, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.25*65536));
		if (Status == 0)			//设定SIGMA范围		
			Status = VL53L0X_SetLimitCheckValue(pMyDevice, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(18*65536));			
		if (Status == 0)			//设定完整测距最长时间
			Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice, 200000);
		if (Status == 0)			//设定VCSEL脉冲周期
			Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 14);
		if (Status == 0) 			//设定VCSEL脉冲周期范围 
			Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 10);
	}
	if(accuracy_mode == 2)			//长距离测量模式
	{
		if (Status == 0)			//设定信号速率范围范围 
			Status = VL53L0X_SetLimitCheckValue(pMyDevice,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,(FixPoint1616_t)(0.1*65536));
		if (Status == 0)			//设定SIGMA范围 
			Status = VL53L0X_SetLimitCheckValue(pMyDevice,VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,(FixPoint1616_t)(60*65536));
		if (Status == 0)			//设定完整测距最长时间 
			Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice,33000);
		if (Status == 0)		    //设定VCSEL脉冲周期
			Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);
		if (Status == 0) 			//设定VCSEL脉冲周期范围 
			Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);
	}
	if(accuracy_mode == 3)		//高速测量模式
	{
		if (Status == 0)			//设定信号速率范围范围
			Status = VL53L0X_SetLimitCheckValue(pMyDevice,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,(FixPoint1616_t)(0.25*65536));
		if (Status == 0)			//设定SIGMA范围 
			Status = VL53L0X_SetLimitCheckValue(pMyDevice,VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,(FixPoint1616_t)(32*65536));
		if (Status == 0)			//设定完整测距最长时间
			Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice, 20000);
		if (Status == 0)		    //设定VCSEL脉冲周期
			Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 14);
		if (Status == 0) 		    //设定VCSEL脉冲周期范围 
			Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 10);
	}
	
    return Status;   //返回0
}

/*********************************************************************
 *  函数名称：int8_t Wl_laserdis_calibration_xtalk_init(VL53L0X_Dev_t *pMyDevice)
 *  函数功能：模块校准初始化
 *  形    参：*pMyDevice：设备结构体，保存了设备的通信信息
 *  输    出：初始化状态：0：成功	其它：失败
 *  备    注：如有条件，请将校准结果写入外部EEPROM或FLASH当中
 ********************************************************************/
int8_t Wl_laserdis_calibration_xtalk_init(VL53L0X_Dev_t *pMyDevice)
{
    int8_t Status = 0;

	uint32_t XTalkCalDistance = 100;
	uint32_t XTalkCompensationRateMegaCps;

    if(Status == 0)
		Status = VL53L0X_StaticInit(pMyDevice);
        
    if(Status == 0)	/*  执行串扰校准  */	
		Status = VL53L0X_PerformXTalkCalibration(pMyDevice,XTalkCalDistance,&XTalkCompensationRateMegaCps);

	if(Status == 0)	//设定串扰校准值
		Status = VL53L0X_SetXTalkCompensationRateMegaCps(pMyDevice,XTalkCompensationRateMegaCps);
			
    return Status;
}
/*********************************************************************
 *  函数名称：int8_t Wl_laserdis_calibration_offset_init(VL53L0X_Dev_t *pMyDevice)
 *  函数功能：模块校准初始化
 *  形    参：*pMyDevice：设备结构体，保存了设备的通信信息
 *  输    出：初始化状态：0：成功	其它：失败
 *  备    注：如有条件，请将校准结果写入外部EEPROM或FLASH当中
 ********************************************************************/
int8_t Wl_laserdis_calibration_offset_init(VL53L0X_Dev_t *pMyDevice)
{
    int8_t Status = 0;

	uint32_t CalDistanceMilliMeter = 100<<16;
	int32_t  OffsetMicroMeter = 30000;

    if(Status == 0)
		Status = VL53L0X_StaticInit(pMyDevice);
        
    if(Status == 0)	/*  执行偏移校准  */	
		Status = VL53L0X_PerformOffsetCalibration(pMyDevice,CalDistanceMilliMeter,&OffsetMicroMeter);

	if(Status == 0)	//设定偏移校准值
		Status = VL53L0X_SetOffsetCalibrationDataMicroMeter(pMyDevice,OffsetMicroMeter);
			
    return Status;
}

/*********************************************************************
 *  函数名称：int8_t Wl_laserdis_calibration_init(VL53L0X_Dev_t *pMyDevice)
 *  函数功能：模块校准初始化
 *  形    参：*pMyDevice：设备结构体，保存了设备的通信信息
 *  输    出：初始化状态：0：成功	其它：失败
 *  备    注：如有条件，请将校准结果写入外部EEPROM或FLASH当中
 ********************************************************************/
int8_t Wl_laserdis_calibration_init(VL53L0X_Dev_t *pMyDevice)
{
    int8_t Status = 0;

    uint32_t refSpadCount;
    uint8_t  isApertureSpads;
    uint8_t  VhvSettings;
    uint8_t  PhaseCal;
        
    if(Status == 0)	/*  执行SPAD（单光子雪崩二极管）校准  */	
		Status = VL53L0X_PerformRefSpadManagement(pMyDevice,&refSpadCount, &isApertureSpads);
    if(Status == 0)	/*  执行参考校准  */
		Status = VL53L0X_PerformRefCalibration(pMyDevice, &VhvSettings, &PhaseCal);

	if(Status == 0)	//设定Spads校准值
		Status = VL53L0X_SetReferenceSpads(pMyDevice,refSpadCount,isApertureSpads);
	if(Status == 0)	//设定Ref校准值
		Status = VL53L0X_SetRefCalibration(pMyDevice,VhvSettings,PhaseCal);
			
    return Status;
}

/*********************************************************************
 *  函数名称：int8_t Wl_laserdis_getdeviceinfo(VL53L0X_Dev_t *pMyDevice)
 *  函数功能：获取设备信息
 *  形    参：*pMyDevice：设备结构体，保存了设备的通信信息
 *  输    出：初始化状态：0：成功	其它：失败
 *  备    注：无
 ********************************************************************/
int8_t Wl_laserdis_getdeviceinfo(VL53L0X_Dev_t *pMyDevice)
{
	int8_t Status = 0;  
	VL53L0X_DeviceInfo_t  vl53l0x_dev_info;       //设备ID结构体，保存设备信息

    if(Status == 0)
		Status = VL53L0X_GetDeviceInfo(pMyDevice, &vl53l0x_dev_info);		//获取设备信息

    printf("VL53L0X_GetDeviceInfo:\n");
    printf("Device Name : %s\n", vl53l0x_dev_info.Name);     	//设备名
    printf("Device Type : %s\n", vl53l0x_dev_info.Type);    	//产品类型VL53L0X = 1, VL53L1 = 2 
    printf("Device ID : %s\n", vl53l0x_dev_info.ProductId);   	// 设备ID
    printf("ProductRevisionMajor : %d\n", vl53l0x_dev_info.ProductRevisionMajor);
    printf("ProductRevisionMinor : %d\n", vl53l0x_dev_info.ProductRevisionMinor);

    if ((vl53l0x_dev_info.ProductRevisionMajor != 1) && (vl53l0x_dev_info.ProductRevisionMinor != 1))	//通信失败
	{
        Status = VL53L0X_ERROR_NOT_SUPPORTED;
        return Status;
    }

	return Status;
}

/*********************************************************************
 *  函数名称：int8_t Wl_laserdis_init
 *  函数功能：模块初始化
 *  形    参：无
 *  输    出：初始化状态：0：成功	其它：失败
 *  备    注：无
 ********************************************************************/
int8_t Wl_laserdis_init(void)
{
    int8_t Status = 0;   
	
    VL53L0X_Dev_t *pMyDevice = &wl_laserdis_dev; 
	
    pMyDevice->I2cDevAddr      =  0x52;           //iic地址
    pMyDevice->comms_type      =  1;              //iic=1
    pMyDevice->comms_speed_khz =  400;            //iic速率
    
    VL53L0X_i2c_init();								//IIC配置    

	if(Status == 0)
		Status = VL53L0X_DataInit(pMyDevice); 		//数据初始化

	if(Status == 0)
		Status = VL53L0X_StaticInit(pMyDevice); 		//静态初始化

	//以下语句目的主要用于调试观看设备信息，实际应用中为优化代码，可直接屏蔽
	//所有打印信息的语句都仅应用于调试，以下不在重复
//	if(Status == 0)
//		Status = Wl_laserdis_getdeviceinfo(pMyDevice); 		

	/********************************************************************
	 * 模块的校准顺序，请按照以下次序依次进行校准
	 ********************************************************************/

	//SPAD校准和参考校准，无环境要求，可直接执行
	if(Status == 0)
		Status = Wl_laserdis_calibration_init(pMyDevice); 	
	
	//偏移校准，要求环境在无红外干扰的环境下，放置一张白色的纸板于模块正前方10cm处（反光85%以上）
	if(Status == 0)
		Status = Wl_laserdis_calibration_offset_init(pMyDevice);
	
	//偏移校准与串扰校准之间需延时一小段时间进行模块内部调整（适应环境）
	delay_ms(500);	
	
	//串扰校准，要求环境在无红外干扰的环境下，放置一张灰色的纸板于模块正前方10cm处（反光15%以下）
	if(Status == 0)
		Status = Wl_laserdis_calibration_xtalk_init(pMyDevice); 	

	//校准完成后，配置测量模式
	if(Status == 0)
		Status = Wl_laserdis_measure_init(pMyDevice, 0,1); 		
	
    return Status;
}

/*********************************************************************
 *  函数名称：unsigned int WL_LasterDisSignleRangle
 *  函数功能：加权测量结果
 *  形    参：distance
 *  输    出：测量修正后的结果
 *  备    注：无
 ********************************************************************/
unsigned int WL_LasterDisSignleRangleDistance(unsigned int distance)
{
	static unsigned int distancetemp[8] = { 0 };
	static unsigned char distancecount = 0;
	unsigned char suni = 0;

	//临近两次测量值相差150mm以上
	if(((distance > distancetemp[4]) && (distance - distancetemp[4] > 150))

		|| ((distancetemp[4] > distance) && (distancetemp[4] - distance > 150)))	
	{
		distancecount = 0;
		for(suni=0; suni<5; suni++)
		{
			distancetemp[suni] = 0;
		}
	}

	distancetemp[5] = distance;			//保存本次测量结果
	distancecount += 1;					//连续有效测量结果次数
	if(distancecount > 4)
	{
		distancecount = 5;
	}
	
	for(suni=0; suni<5; suni++)			//历史测量结果移位并累加
	{
		distancetemp[suni] = distancetemp[suni+1];
		distancetemp[6] += distancetemp[suni];
	}

	distancetemp[7] = distancetemp[6] / distancecount;
	distancetemp[6] = 0;				//累加值清零，用于下一次计算

	return distancetemp[7];
}





